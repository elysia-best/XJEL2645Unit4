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

namespace Engine {

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

 public:

  
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
   * @Name    Finalized
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
  virtual void Finalized();
};
}  // namespace Engine

#endif
