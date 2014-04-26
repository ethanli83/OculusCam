//
// Created by Ethan Li on 26/04/2014.
// Copyright (c) 2014 Ethan.Li. All rights reserved.
//


#include <vector>
#include "SceneObject.h"

#ifndef __Scene_H_
#define __Scene_H_


class Scene {
public:
    void addObject(SceneObject object);
    std::vector<SceneObject> objects;

private:

};


#endif //__Scene_H_
