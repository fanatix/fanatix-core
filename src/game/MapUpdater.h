/*
 * Copyright (C) 2005-2008 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _MAP_UPDATER_H_INCLUDED
#define _MAP_UPDATER_H_INCLUDED


#include <ace/Thread_Mutex.h>
#include <ace/Condition_Thread_Mutex.h>

#include "DelayExecutor.h"

class Map;

class MapUpdater
{
public:
  MapUpdater ();
  virtual ~MapUpdater ();
  
  friend class MapUpdateRequest;

  /// schedule update on a map, the update will start
  /// as soon as possible ,
  /// it may even start before the call returns
  int schedule_update(Map& map, ACE_UINT32 diff);
  
  /// Wait untill all pending updates finish
  int wait ();
  
  /// Start the worker threads
  int activate (size_t num_threads);
  
  /// Stop the worker threads
  int deactivate (void);
  
private:
  /// hook called by worker threads
  void update_finished ();
  
  DelayExecutor m_executor;
  ACE_Condition_Thread_Mutex m_condition;
  ACE_Thread_Mutex m_mutex;
  size_t pedning_requests;
};

#endif //_MAP_UPDATER_H_INCLUDED
