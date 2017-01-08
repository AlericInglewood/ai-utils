/**
 * @file
 * @brief Declaraction of class GlobalObjectManager.
 *
 * Copyright (C) 2014 - 2017  Carlo Wood.
 *
 * RSA-1024 0x624ACAD5 1997-01-26                    Sign & Encrypt
 * Fingerprint16 = 32 EC A7 B6 AC DB 65 A6  F6 F6 55 DD 1C DC FF 61
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \class GlobalObjectManager
 * \brief Global objects manager singleton class.
 *
 * This singleton is used by Global<> to keep track of the number
 * of global instances and their destruction.
 */

#include "Global.h"		// Must be included before GlobalObjectManager.h

#ifndef UTILS_GLOBAL_OBJECT_MANAGER_H
#define UTILS_GLOBAL_OBJECT_MANAGER_H

#include "Singleton.h"

#ifndef USE_PCH
#include <vector>
#if defined(CWDEBUG) && CWDEBUG_ALLOC
#include <libcwd/private_internal_stringbuf.h>
#endif
#include "debug.h"
#endif

class GlobalObjectManager : public Singleton<GlobalObjectManager>
{
  friend_Instance;
private:
#if defined(CWDEBUG) && CWDEBUG_ALLOC
  typedef std::vector<utils::_internal_::GlobalObject*,
      libcwd::_private_::auto_internal_allocator::rebind<utils::_internal_::GlobalObject*>::other> globalObjects_type;
#else
  typedef std::vector<utils::_internal_::GlobalObject*> globalObjects_type;
#endif
  globalObjects_type globalObjects;
  int number_of_global_objects;

private:
  ~GlobalObjectManager() { }
  GlobalObjectManager(GlobalObjectManager const&) : Singleton<GlobalObjectManager>() { }	// Never used

private:
  template<class TYPE, int inst, class CONVERTER> friend class Global;
  void global_constructor_called() { ++number_of_global_objects; }
  template<class TYPE, int inst> friend class utils::_internal_::GlobalBase;
  void global_destructor_called() {
#ifdef DEBUGGLOBAL
    if (!after_global_constructors)
      DoutFatal(dc::core, "When you link with libcw, you should not define your own main(). "
                         "Perhaps you intended to only link with libcwd?");
#endif
    if (--number_of_global_objects == 0)
      deleteGlobalObjects();
  }
  void deleteGlobalObjects();

public:
  void registerGlobalObject(utils::_internal_::GlobalObject* globalObject);

#ifndef DEBUGGLOBAL
private:
  GlobalObjectManager() : number_of_global_objects(0) { }
#else
private:
  static bool after_global_constructors;
  GlobalObjectManager() : number_of_global_objects(0) { }
public:
  static void main_entered();	// Should be called at the top of main() - sets `after_global_constructors' and does checking.
  static bool is_after_global_constructors() { return after_global_constructors; }
#endif
};

#endif // UTILS_GLOBAL_OBJECT_MANAGER_H
