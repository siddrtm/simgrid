/* Copyright (c) 2004-2015. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <boost/intrusive/list.hpp>
#include <deque>

#include <xbt/base.h>

#include "simgrid/s4u/VirtualMachine.hpp"
#include "src/surf/HostImpl.hpp"

#ifndef VM_INTERFACE_HPP_
#define VM_INTERFACE_HPP_

#define GUESTOS_NOISE 100 // This value corresponds to the cost of the global action associated to the VM
                          // It corresponds to the cost of a VM running no tasks.

namespace simgrid {
namespace surf {

/***********
 * Classes *
 ***********/

class XBT_PRIVATE VMModel;
class XBT_PRIVATE VirtualMachineImpl;

/*************
 * Callbacks *
 *************/

/** @ingroup SURF_callbacks
 * @brief Callbacks fired after VM creation. Signature: `void(VirtualMachine*)`
 */
extern XBT_PRIVATE simgrid::xbt::signal<void(simgrid::surf::VirtualMachineImpl*)> onVmCreation;

/** @ingroup SURF_callbacks
 * @brief Callbacks fired after VM destruction. Signature: `void(VirtualMachine*)`
 */
extern XBT_PRIVATE simgrid::xbt::signal<void(simgrid::surf::VirtualMachineImpl*)> onVmDestruction;

/** @ingroup SURF_callbacks
 * @brief Callbacks after VM State changes. Signature: `void(VirtualMachine*)`
 */
extern XBT_PRIVATE simgrid::xbt::signal<void(simgrid::surf::VirtualMachineImpl*)> onVmStateChange;

/************
 * Resource *
 ************/

/** @ingroup SURF_vm_interface
 * @brief SURF VM interface class
 * @details A VM represent a virtual machine
 */
class VirtualMachineImpl : public HostImpl {
public:
  explicit VirtualMachineImpl(s4u::Host* piface, s4u::Host* host);
  ~VirtualMachineImpl();

  /** @brief Suspend the VM */
  virtual void suspend();

  /** @brief Resume the VM */
  virtual void resume();

  /** @brief Save the VM (Not yet implemented) */
  virtual void save();

  /** @brief Restore the VM (Not yet implemented) */
  virtual void restore();

  /** @brief Migrate the VM to the destination host */
  virtual void migrate(s4u::Host* dest);

  /** @brief Get the physical machine hosting the VM */
  s4u::Host* getPm();

  sg_size_t getRamsize();

  virtual void setBound(double bound);

  void getParams(vm_params_t params);
  void setParams(vm_params_t params);

  /* The vm object of the lower layer */
  CpuAction* action_ = nullptr;

  /* Dirty pages stuff */
  int dp_enabled                     = 0;
  xbt_dict_t dp_objs                 = nullptr;
  double dp_updated_by_deleted_tasks = 0;

protected:
  simgrid::s4u::Host* hostPM_;

public:
  e_surf_vm_state_t getState();
  void setState(e_surf_vm_state_t state);
  static std::deque<VirtualMachineImpl*> allVms_;

  bool isMigrating = false;

private:
  s_vm_params_t params_;

protected:
  e_surf_vm_state_t vmState_ = SURF_VM_STATE_CREATED;
};

/*********
 * Model *
 *********/
/** @ingroup SURF_vm_interface
 * @brief SURF VM model interface class
 * @details A model is an object which handle the interactions between its Resources and its Actions
 */
class VMModel : public HostModel {
public:
  void adjustWeightOfDummyCpuActions() override{};

  double nextOccuringEvent(double now) override;
  void updateActionsState(double /*now*/, double /*delta*/) override{};
};
}
}

#endif /* VM_INTERFACE_HPP_ */