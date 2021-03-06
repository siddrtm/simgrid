/* Copyright (c) 2007-2018. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include "simgrid/s4u/Host.hpp"

#include "simgrid/kernel/resource/Action.hpp"
#include "src/kernel/activity/SleepImpl.hpp"
#include "src/kernel/context/Context.hpp"

#include "src/simix/ActorImpl.hpp"
#include "src/simix/popping_private.hpp"
#include "src/surf/surf_interface.hpp"

XBT_LOG_EXTERNAL_DEFAULT_CATEGORY(simix_process);

void simgrid::kernel::activity::SleepImpl::suspend()
{
  surf_sleep->suspend();
}

void simgrid::kernel::activity::SleepImpl::resume()
{
  surf_sleep->resume();
}

void simgrid::kernel::activity::SleepImpl::post()
{
  while (not simcalls_.empty()) {
    smx_simcall_t simcall = simcalls_.front();
    simcalls_.pop_front();

    e_smx_state_t result;
    switch (surf_sleep->get_state()) {
      case simgrid::kernel::resource::Action::State::FAILED:
        simcall->issuer->context_->iwannadie = 1;
        result                              = SIMIX_SRC_HOST_FAILURE;
        break;

      case simgrid::kernel::resource::Action::State::FINISHED:
        result = SIMIX_DONE;
        break;

      default:
        THROW_IMPOSSIBLE;
        break;
    }
    if (simcall->issuer->host_->is_off()) {
      simcall->issuer->context_->iwannadie = 1;
    }
    simcall_process_sleep__set__result(simcall, result);
    simcall->issuer->waiting_synchro = nullptr;
    if (simcall->issuer->suspended_) {
      XBT_DEBUG("Wait! This process is suspended and can't wake up now.");
      simcall->issuer->suspended_ = 0;
      simcall_HANDLER_process_suspend(simcall, simcall->issuer);
    } else {
      SIMIX_simcall_answer(simcall);
    }
  }

  SIMIX_process_sleep_destroy(this);
}
