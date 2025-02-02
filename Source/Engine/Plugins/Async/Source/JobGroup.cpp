//
// Created by Dev on 3/29/2018.
//

#include <Async/JobGroup.h>
#include <Async/Detail/AsyncHelper.h>
#include <Async/Framework.h>

DJINN_NS2(Async);

void JobGroup::Submit() {
    Detail::GetFramework()->Schedule(scheduledJobs.Size(), scheduledJobs.Ptr(), Async::kDefault);
    Detail::GetFramework()->Schedule(yieldedJobs.Size(), yieldedJobs.Ptr());
    scheduledJobs.Reset();
    yieldedJobs.Reset();
}
