#pragma once

//---------------------------------------------------------------------------//
// Copyright (C) Djinn Engine Team - All Rights Reserved	    			 //
// Unauthorized copying of this file, via any medium is strictly prohibited  //
// Proprietary and confidential 											 //
//  * Written by Miguel Petersen    										 //
//---------------------------------------------------------------------------//

#include <NyLang/Core/Exception.h>
#include <NyLang/Frontend/Token/Token.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <list>
#include <NyLang/Frontend/Symbol/SymbolPair.h>
#include <NyLang/Frontend/Symbol/Symbol.h>
#include <unordered_map>
#include <NyLang/Core/Context.h>

namespace NyLang {
    class SymbolTree;
    class Symbol;

    // Common exceptions
    LANG_EXCEPTION(SymbolAmbiguous);
    LANG_EXCEPTION(NoSymbol);

    // Max dependency level
    static constexpr int kMaxDependencyLevel = std::numeric_limits<int>::max();

    // Base node for all ast nodes
    class Node : public IUnknown {
    public:
        Node(const Token &tok, const std::list<Node*>& operands);

        /// Perform a templated copy of this node
        /// \return
        virtual Node* TemplatedCopy(Context &context) = 0;

        /// Clear this node and all of its children
        virtual void Clear();

        /// Resolve the deferred ness of this node
        /// \return
        bool ResolveDeferred();

        /// Is this node symbolically ambiguous?
        /// \return
        bool IsSymbolAmbiguous();

        /// Get unambiguous symbol, may throw an exception
        /// \return
        Symbol* GetUnambiguousSymbol();

        /// Does this node have any symbols?
        /// \return
        bool HasSymbol();

        /// Get symbol anchor of this node
        /// \param tree
        /// \return
        SymbolAnchor GetAnchor(SymbolTree *tree = nullptr);

        /// Get symbol by type of this node
        /// \return
        // Get symbol by type
        template<class T>
        SymbolPair<T> GetSymbol() {
            for (auto &symbol : symbols) {
                if (auto &&com = symbol->GetComponent<T>()) {
                    return {symbol, com};
                }
            }
            return {};
        }

        /// Add symbol to this node, may be null
        /// \param Ptr
        void AddSymbol(Symbol*Ptr);

        /// Replace symbol by pointer
        /// \param ptr
        /// \param replacement
        void ReplaceSymbol(Symbol* ptr, Symbol* replacement);

        /// Add multiple symbols to this node
        /// \param Ptrs
        void AddSymbols(const std::list<Symbol*> &Ptrs) {
            for (const auto &ptr : Ptrs) {
                AddSymbol(ptr);
            }
        }

        /// Add a frame
        /// \param frame
        /// \return
        template<typename T>
        T* AddFrame(T*frame) {
            frames[frame->GetClassID()] = static_cast<IUnknown*>(frame);
            return frame;
        }

        /// Add a frame
        /// \param id
        /// \param frame
        /// \return
        template<typename T>
        T* AddFrame(ClassID id, T*frame) {
            frames[id] = static_cast<IUnknown*>(frame);
            return frame;
        }

        /// Get a frame with a type
        /// \return
        template<typename T>
        T* GetFrame() {
            T* frame = static_cast<T*>(frames[T::kCID]);
            if (!frame && parent) {
                frame = parent->GetFrame<T>();
            }
            return frame;
        }

        /// Remove a frame with a type
        template<typename T>
        void RemoveFrame() {
            frames.erase(T::kCID);
        }

        /// Remove a frame
        /// \param id
        void RemoveFrame(ClassID id) {
            frames.erase(id.Get());
        }

        // Node Token
        Token tok;

        // Symbols of this node
        std::list<Symbol*> symbols;

        // Some nodes depend on a templated symbol
        // Note: May be ignored
        bool isDeferred = false;

        // Is this node dependent on some future scope?
        int dependencyLevel = 0;

        // Completed stages
        int stages = 0;

        // Is this node complete?
        bool isIncomplete = false;

        // Does this node require an end of statement token?
        bool requireEOS = true;

        ///  Move to other other nodes
        /// \param frames
        /// \param args
        template<typename... A>
        void Move(const Vector<IUnknown*> &frames, const A &... args) {
            MoveImpl(frames, args...);
        }

        // Node operands
        std::list<Node*> operands;

        // Parent node
        Node* parent = nullptr;

    protected:
        /// Move to other other nodes
        /// \param frame
        /// \param t0
        template<typename T0>
        void MoveImpl(const Vector<IUnknown*> &frames, const T0 &t0) {
            // Null nodes are allowed
            if (t0) {
                for (auto&& frame : frames) {
                    t0->AddFrame(frame);
                }
                t0->parent = this;
            }
        }

        /// Move to other other nodes
        /// \param frame
        /// \param t0
        /// \param t1
        /// \param tx
        template<typename T0, typename T1, typename... T>
        void MoveImpl(const Vector<IUnknown*> &frames, const T0 &t0, const T1 &t1, const T &... tx) {
            MoveImpl(frames, t0);
            MoveImpl(frames, t1, tx...);
        }

        // Frames
        UnorderedMap<ClassID::Type, IUnknown*> frames;
    };
}