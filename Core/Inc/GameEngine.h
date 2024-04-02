/*
 * (C) Copyright 2024 Yuanzhen Gan. All Rights Reserved.
 * Description: GameEngine.h
 * Author: Yuanzhen Gan
 * Date: 2024-04-01
 * Modify Record:
 */

#ifndef __GAME_ENGINE_H_
#define __GAME_ENGINE_H_

#include "matrix.h"
#include <vector>

namespace Engine {

typedef struct{
	BaseBehaviour* ptr;
	bool delete_on_destroy;
} AttachedComponent_t;

class BaseBehaviour {
 public:
  /**
   * @Name    BaseBehaviour
   * @brief   Constructor of BaseBehaviour. Should not be directly used in child
   *classes.
   * @param   : None
   * @retval  : None
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
  BaseBehaviour();

  /**
   * @Name    ~BaseBehaviour
   * @brief   Destruction function fot class BaseBehaviour. It is recommneded to
   *do destruction in Finalized function in child classes.
   * @param   : None
   * @retval  : None
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
  virtual ~BaseBehaviour();

  /**
   * @Name    AttachComponent
   * @brief   Attach a component to this object.
   * @param   ptr: Pointer of component to be attached
   **			    delete_on_destroy:  Whether to delete this component on the destruction of this object
   * @retval  : None
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
	void AttachComponent(const BaseBehaviour* &ptr, bool delete_on_destroy);

 public:
	/**
   * @Name    Elements_ptr_list
   * @brief   All the elements/components attache to a list should be stored here.
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
   std::vector<AttachedComponent_t> Elements_ptr_list;

  /**
   * @Name    IsDisabled
   * @brief   Indicates whether this object/component is disabled.
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
	 bool IsDisabled;
 protected:
  /**
   * @Name    Start
   * @brief   Executed on the creation of the object, i.e., first update.
   * @param   : None
   * @retval  : None
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
  virtual void Start();

  /**
   * @Name    Update
   * @brief   Executed on each frame update. The frame is refreshed when all the
   *objects finished the update.
   * @param   : None
   * @retval  : None
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
  virtual void Update();

  /**
   * @Name    OnDestroyed
   * @brief   Executed upon the destruction of the object.
   * @param   : None
   * @retval  : None
   * @author  Yuanzhen Gan
   * @Date    2024-04-02
   * 1. ...
   * <modify staff>:
   * <data>        :
   * <description> :
   * 2. ...
   **/
  virtual void OnDestroyed();
};
}  // namespace Engine

#endif
