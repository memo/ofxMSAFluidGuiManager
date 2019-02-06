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
            bool			enabled;
            bool            doDraw;
            float			velMult;
            float			dyeAmount;
            float			maxSpeed;
            float			viscocity;
            int				fluidStep;
            int				cellsX;
            int				cellsY;
            float			randomForce;
            bool			resizeFluid;
            
            DrawerGl	fluidDrawer;
            DrawerGl	fluidMotion;
            DrawerGl	fluidVectors;
            Solver		fluid;
            
            void reset();
            void setup();
            void update();
            void draw(float x, float y, float w, float h);
            
            // offset (-1...1) and then scale in place
            void injectVel(ofBaseHasPixels &maskPixels, Vec2f *velArray, Vec2f velDim, Vec2f offset=Vec2f(0), Vec2f scale=Vec2f(1), ofFloatColor* pcolor=NULL);
            
        protected:

            void updateSize();
        };
    }
}
