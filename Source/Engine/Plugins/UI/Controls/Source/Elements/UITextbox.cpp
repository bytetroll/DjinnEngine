//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 8/23/2018.
//

#include <UI/Elements/UITextbox.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Game/IActorSystem.h>
#include <UI/Elements/UIImage.h>
#include <Data/IAssetHost.h>
#include <Data/IFontAsset.h>
#include <Data/ITextureAsset.h>
#include <Base/DefaultLogs.h>
#include <Graphics/UIDrawPacket.h>
#include <UI/ElementComponents/UIInputComponent.h>
#include <UI/UIInputActionBuffer.h>

DJINN_NS2(UI);

UITextbox::UITextbox(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIElement(registry, outer, host) {

}

void UITextbox::StaticInitialize() {
    SetTransform(CreateComponent<UILayoutTransformComponent>());

    // Renderer
    renderer = CreateComponent<Renderer>();

    // Get asset worldHost
    if (!registry->GetPipeline()->GetInterface(&host)) {
        ErrorLog(this).Write("Expected an asset worldHost to be installed");
        return;
    }

    // Default assets
    renderer->font = host->CompileAssetAsync<Data::IFontAsset>("Runtime/Engine/Fonts/Akrobat-Black.otf"_path);
    renderer->texture = host->CompileAssetAsync<Data::ITextureAsset>("Runtime/Engine/UI/TextboxMB.png"_path);

    // Register input
    auto input = CreateComponent<UIInputComponent>();
    {
        input->mouseClicked.Add(this, [=](const Game::InputEvent::MousePressed &event, UIInputActionBuffer *buffer) {

        });

        // Pressed event
        input->mousePressed.Add(this, [=](const Game::InputEvent::MousePressed &event, UIInputActionBuffer *buffer) {
            renderer->isPressing = true;

            float dx = event.position.x - GetTransform()->GetAbsolutePosition().x;
            renderer->selectionIndices.x = renderer->selectionIndices.y = renderer->GetGlyphIndex(dx);

            buffer->Add(UIInputAction::BindMouseFocus(input->GetWeakPtr()));
        });

        // Release event
        input->mouseReleased.Add(this, [=](const Game::InputEvent::MouseReleased &event, IUIElement *element, UIInputActionBuffer *buffer) {
            if (renderer->isPressing) {
                renderer->isPressing = false;
                renderer->isEditing = true;

                buffer->Add(UIInputAction::BindKeyboardFocus(input->GetWeakPtr()));

                buffer->Add(UIInputAction::UnbindMouseFocus());
            }
        });

        // Moved event
        input->mouseMoved.Add(this, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *buffer) {
            if (renderer->isPressing && renderer->glyphRenders.Size() > 0) {
                auto glyphOffset = renderer->glyphRenders.Last().offset + renderer->glyphRenders.Last().size * renderer->fontSize;

                float dx = event.position.x - GetTransform()->GetAbsolutePosition().x;
                renderer->selectionIndices.y = renderer->GetGlyphIndex(dx);
            }
        });

        // Hover begin
        input->beginHover.Add(this, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *buffer) {
            renderer->isHovering = true;
        });

        // Hover end
        input->endHover.Add(this, [=](const Game::InputEvent::MouseMoved &event, UIInputActionBuffer *buffer) {
            renderer->isHovering = false;
        });

        // Character
        input->characterTyped.Add(this, [=](const Game::InputEvent::Character &event, UIInputActionBuffer *buffer) {
            switch (event.virtualKeyCode) {
                default:
                    break;
                case Platform::VirtualKeyCode::eArrowLeft: {
                    if (renderer->selectionIndices.x == renderer->selectionIndices.y && renderer->selectionIndices.x != 0) {
                        renderer->selectionIndices.y = --renderer->selectionIndices.x;
                    } else {
                        renderer->selectionIndices.y = renderer->selectionIndices.x;
                    }
                    break;
                }
                case Platform::VirtualKeyCode::eArrowRight: {
                    if (renderer->selectionIndices.x == renderer->selectionIndices.y && renderer->selectionIndices.x != static_cast<Int32>(renderer->text.Length())) {
                        renderer->selectionIndices.x = ++renderer->selectionIndices.y;
                    } else {
                        renderer->selectionIndices.x = renderer->selectionIndices.y;
                    }
                    break;
                }
                case Platform::VirtualKeyCode::eBackspace: {
                    if (renderer->selectionIndices.x == renderer->selectionIndices.y) {
                        if (renderer->selectionIndices.x != 0) {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.x - 1) + Core::String(renderer->text, renderer->selectionIndices.x, static_cast<Int32>(renderer->text.Length()));
                            renderer->selectionIndices = {renderer->selectionIndices.x - 1, renderer->selectionIndices.x - 1};
                        }
                    } else {
                        if (renderer->selectionIndices.x > renderer->selectionIndices.y) {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.y) + Core::String(renderer->text, renderer->selectionIndices.x, static_cast<Int32>(renderer->text.Length()));
                        } else {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.x) + Core::String(renderer->text, renderer->selectionIndices.y, static_cast<Int32>(renderer->text.Length()));
                        }
                        renderer->selectionIndices = {renderer->selectionIndices.x, renderer->selectionIndices.x};
                    }
                    break;
                }
                case Platform::VirtualKeyCode::eDelete: {
                    if (renderer->selectionIndices.x == renderer->selectionIndices.y) {
                        if (renderer->selectionIndices.x == static_cast<Int32>(renderer->text.Length()) - 1) {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.x);
                        } else if (renderer->selectionIndices.x < static_cast<Int32>(renderer->text.Length()) - 1) {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.x) + Core::String(renderer->text, renderer->selectionIndices.x + 1, renderer->text.Length());
                        }
                    } else {
                        if (renderer->selectionIndices.x > renderer->selectionIndices.y) {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.y) + Core::String(renderer->text, renderer->selectionIndices.x, renderer->text.Length());
                        } else {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.x) + Core::String(renderer->text, renderer->selectionIndices.y, renderer->text.Length());
                        }
                        renderer->selectionIndices = {renderer->selectionIndices.x, renderer->selectionIndices.x};
                    }
                    break;
                }
                case Platform::VirtualKeyCode::eCharacter: {
                    if (renderer->selectionIndices.x == renderer->selectionIndices.y) {
                        renderer->text.Insert(event.key, renderer->selectionIndices.x);
                    } else {
                        if (renderer->selectionIndices.x > renderer->selectionIndices.y) {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.y) + event.key + Core::String(renderer->text, renderer->selectionIndices.x, renderer->text.Length());
                        } else {
                            renderer->text = Core::String(renderer->text, 0, renderer->selectionIndices.x) + event.key + Core::String(renderer->text, renderer->selectionIndices.y, renderer->text.Length());
                        }
                    }
                    renderer->selectionIndices = {renderer->selectionIndices.x + 1, renderer->selectionIndices.x + 1};
                    break;
                }
            }
            MarkAsUncached();
        });
    }

    // Default size
    GetTransform()->SetDesiredSize({100, 24});

    // ...
    MarkAsUncached();
}

void UITextbox::SetFont(const Core::StringView &path) {
    renderer->font = host->CompileAssetAsync<Data::IFontAsset>(Base::ToPath(path));
    renderer->MarkAsInvalidated();
    MarkAsUncached();
}

void UITextbox::SetText(const Core::StringView &text) {
    renderer->text = text.ToString();
    MarkAsUncached();
}

Core::StringView UITextbox::GetText() const {
    return renderer->text;
}

void UITextbox::SetHint(const Core::StringView &text) {
    renderer->hint = text.ToString();
    MarkAsUncached();
}

Core::StringView UITextbox::GetHint() const {
    return renderer->hint;
}

bool UITextbox::Cache() {
    // Must have font attached
    if (!renderer->font.IsValid()) {
        return true;
    }

    // Ready?
    Data::IFontAsset *asset;
    if (!(asset = renderer->font.GetRaw())) {
        return false;
    }

    renderer->glyphRenders.Clear();
    renderer->hintGlyphRenders.Clear();

    if (renderer->text.Empty()) {
        // Ensure size
        renderer->hintGlyphRenders.Resize(renderer->hint.Length());

        // Prepare request
        Data::FontRenderRequest request({renderer->hintGlyphRenders});

        // Render glyphs
        if (!asset->RenderGlyphs(
                renderer->hint,
                Data::FontRenderConfig()
                        .Size(renderer->fontSize)
                        .LineSeperation(renderer->lineSeperation),
                request
        )) {
            ErrorLog(this).Write("Failed to render font glyphs with text: '", renderer->hint, "'");
            return true;
        }

        // Some glyphs may be ignored
        renderer->hintGlyphRenders.Resize(request.glyphs.GetSize());
    } else {
        // Ensure size
        renderer->glyphRenders.Resize(renderer->text.Length());

        // Prepare request
        Data::FontRenderRequest request({renderer->glyphRenders});

        // Render glyphs
        if (!asset->RenderGlyphs(
                renderer->text,
                Data::FontRenderConfig()
                        .Size(renderer->fontSize)
                        .LineSeperation(renderer->lineSeperation),
                request
        )) {
            ErrorLog(this).Write("Failed to render font glyphs with text: '", renderer->text, "'");
            return true;
        }

        // Some glyphs may be ignored
        renderer->glyphRenders.Resize(request.glyphs.GetSize());
    }

    // OK
    return true;
}

UITextbox::Renderer::Renderer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IUIRenderComponent(registry, outer, host) {

}

void UITextbox::Renderer::Build(Graphics::UIDrawPacketBuffer *buffer) {
    auto transform = GetElement()->GetTransform();

    // Ignore no glyphs
    if (!font) {
        return;
    }

    {
        Graphics::UIDrawPacket packet;
        packet.transform = transform->GetPixelTransform();
        packet.brush = Graphics::UIBrushTexture(
                isHovering ? hoverTint : Math::Vector4(1, 1, 1, 1),
                texture,
                GHA::SamplerDesc()
        );
        packet.geometry = Graphics::UIGeometryMarginBox(transform->GetDesiredSize(), {4, 4});
        if (auto parent = transform->GetParent()) {
            packet.viewport = parent->GetViewport();
        }
        buffer->Add(packet);
    }

    // Submit packet
    if (glyphRenders.Size() == 0) {
        Graphics::UIDrawPacket packet;
        packet.transform = Math::Matrix4::Translate({6, 6, 1}) * transform->GetPixelTransform();
        packet.brush = Graphics::UIBrushSDF(Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f), 1.0f / smoothing);
        Graphics::UIGeometryFont geometryFont;
        geometryFont.glyphs = hintGlyphRenders;
        geometryFont.font = font;
        packet.geometry = geometryFont;
        packet.viewport = transform->GetViewport();
        buffer->Add(packet);
    } else {
        Graphics::UIDrawPacket packet;
        packet.transform = Math::Matrix4::Translate({6, 6, 1}) * transform->GetPixelTransform();
        packet.brush = Graphics::UIBrushSDF(color, 1.0f / smoothing);
        Graphics::UIGeometryFont geometryFont;
        geometryFont.glyphs = glyphRenders;
        geometryFont.font = font;
        packet.geometry = geometryFont;
        packet.viewport = transform->GetViewport();
        buffer->Add(packet);
    }

    if ((isPressing || isEditing)) {
        auto glyphOffset = glyphRenders.Size() == 0 ? 0.0f : glyphRenders.Last().offset.x + glyphRenders.Last().size.x * fontSize;

        auto offset = Math::Min(
                (selectionIndices.x >= static_cast<Int32>(glyphRenders.Size())) ? glyphOffset : glyphRenders[selectionIndices.x].offset.x,
                (selectionIndices.y >= static_cast<Int32>(glyphRenders.Size())) ? glyphOffset : glyphRenders[selectionIndices.y].offset.x
        ) + 6;
        auto length = Math::Abs(
                ((selectionIndices.y >= static_cast<Int32>(glyphRenders.Size())) ? glyphOffset : glyphRenders[selectionIndices.y].offset.x) -
                ((selectionIndices.x >= static_cast<Int32>(glyphRenders.Size())) ? glyphOffset : glyphRenders[selectionIndices.x].offset.x)
        ) + 3;

        Graphics::UIDrawPacket packet;
        packet.transform = Math::Matrix4::Translate({offset, 2, 2}) * transform->GetPixelTransform();
        packet.geometry = Graphics::UIGeometryBox({length, transform->GetDesiredSize().y - 4});
        packet.brush = Graphics::UIBrushSolid({0.2, 0.2, 0.2, 0.5});
        packet.viewport = transform->GetViewport();
        buffer->Add(packet);
    }
}
