/*
 *  FluidGuiManager.h
 *  msaCamView
 *
 *  Created by Mehmet Akten on 04/07/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#pragma once

#include "MSAFluid.h"

namespace msa {
namespace fluid {
class GuiManager {
public:
    bool		enabled = true;
    bool        doDraw = true;
    float		velMult = 1.0f;
    bool        applyMask = true;
    float		dyeAmount = 10;
    float		maxSpeed = 0;
    float		viscocity = 0.5;
    int			fluidStep = 1;
    int			cellsX = 160;
    int			cellsY = 120;
    float		randomForce = 0;
    bool		resizeFluid = false;

    DrawerGl	fluidDrawer;
    DrawerGl	fluidMotion;
    DrawerGl	fluidVectors;
    Solver		fluid;

    void reset();
    void setup();
    void update();
    void draw(float x, float y, float w, float h);

    // offset (-1...1) and then scale in place
    void injectVel(ofPixelsRef maskPixels, Vec2f *velArray, Vec2f velDim, Vec2f offset=Vec2f(0), Vec2f scale=Vec2f(1), ofFloatColor* pcolor=NULL);

//    template<typename T>
//    void injectVel(ofPixelsRef maskPixels, ofPixels_<T> velPixels, Vec2f offset=Vec2f(0), Vec2f scale=Vec2f(1), ofFloatColor* pcolor=NULL);

protected:

    void updateSize();
};

//template<typename T>
//void GuiManager::injectVel(ofPixelsRef maskPixels, ofPixels_<T> velPixels, Vec2f offset, Vec2f scale, ofFloatColor* pcolor) {
//    static vector<Vec2f> velArray;
//    if(velArray.size() != velPixels.getWidth() * velPixels.getHeight()) velArray.resize(velPixels.getWidth() * velPixels.getHeight());

//    auto d = velPixels.getData();
//    for(int i=0; i<velArray.size(); i++) velArray[i] = Vec2f(d[i<<1], d[(i<<1)+1]);
//    injectVel(maskPixels, velArray.data(), Vec2f(velPixels.getWidth(), velPixels.getHeight()), offset, scale, pcolor);
//}

}
}
