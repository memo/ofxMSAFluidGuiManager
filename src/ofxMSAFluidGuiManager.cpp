/*
 *  FluidGuiManager.cpp
 *  msaCamView
 *
 *  Created by Mehmet Akten on 04/07/2010.
 *  Copyright 2010 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofxMSAFluidGuiManager.h"
#include "ofxSimpleGuiToo.h"

namespace msa {
	namespace fluid {
        
        void GuiManager::reset() {
            fluid.reset();
        }
        
        void GuiManager::setup() {
            gui.saveToXML();
            
            gui.addPage("FLUID").setXMLName("settings/fluid/fluid.xml");
            gui.addToggle("enabled", enabled);
            gui.addToggle("doDraw", doDraw);
            gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, kDrawCount, (string*)&getDrawModeTitles()[0]);
            
            gui.addToggle("fluidDrawer", fluidDrawer.enabled);
            gui.addToggle("fluidAdditive", fluidDrawer.useAdditiveBlending);
            gui.addSlider("fluidBrightness", fluidDrawer.brightness, 0, 20);
            
            gui.addToggle("vectorsDrawer", fluidVectors.enabled);
            gui.addToggle("vectorsAdditive", fluidVectors.useAdditiveBlending);
            gui.addSlider("vectorsBrightness", fluidVectors.brightness, 0, 1);
            
            gui.addTitle("Vector draw options");
            gui.addSlider("velDrawThreshold", fluidVectors.velDrawThreshold, 0, 1);
            gui.addSlider("velDrawMult", fluidVectors.velDrawMult, 0, 5);
            gui.addSlider("vectorSkipCount", fluidVectors.vectorSkipCount, 0, 10);
            
            gui.addSlider("velMult", velMult, 0, 5);
            gui.addSlider("dyeAmount", dyeAmount, 0, 100);
            gui.addSlider("maxSpeed", maxSpeed, 0, 1);
            gui.addSlider("fluidStep", fluidStep, 0, 20);
            gui.addSlider("cellsX", cellsX, 20, 1024);
            gui.addSlider("cellsY", cellsY, 20, 512);
            gui.addButton("resizeFluid", resizeFluid);
            gui.addSlider("randomForce", randomForce, 0, 1);
            gui.addSlider("viscocity", viscocity, 0.0, 1);
            gui.addSlider("colorDiffusion", fluid.colorDiffusion, 0.0, 0.0003);
            gui.addSlider("fadeSpeed", fluid.fadeSpeed, 0.0, 0.1);
            gui.addSlider("solverIterations", fluid.solverIterations, 1, 20);
            gui.addSlider("deltaT", fluid.deltaT, 0.1, 2);
            gui.addToggle("doRGB", fluid.doRGB);
            gui.addToggle("doVorticityConfinement", fluid.doVorticityConfinement);
            gui.addToggle("wrapX", fluid.wrap_x);
            gui.addToggle("wrapY", fluid.wrap_y);
            
            gui.addContent("Fluid Motion", fluidMotion);
            gui.addContent("Fluid Vectors", fluidVectors);
            
            gui.loadFromXML();
            
            
            if(cellsX == 0) cellsX = 120;
            if(cellsY == 0) cellsY = 90;
            
            fluid.setSize(cellsX, cellsY);
            
            fluidMotion.setup(&fluid);
            fluidMotion.setDrawMode(kDrawMotion);
            
            fluidVectors.setup(&fluid);
            fluidVectors.setDrawMode(kDrawVectors);
            
            fluidDrawer.setup(&fluid);
            
            //		GuiGL::Page &fluidPage = gui.page("FLUID");
            //		fluidPage.addContent("Fluid Motion", fluidMotion);
            //		fluidPage.addContent("Fluid Vectors", fluidVectors);
            
            gui.saveToXML();
        }
        
        
        void GuiManager::update() {
            if(!enabled) return;
            
            fluid.viscocity = viscocity * viscocity * viscocity;
            
            if(resizeFluid) {
                resizeFluid = false;
                fluid.setSize(cellsX, cellsY);
                fluidMotion.setup(&fluid);
                fluidMotion.setDrawMode(kDrawMotion);
                
                fluidVectors.setup(&fluid);
                fluidVectors.setDrawMode(kDrawVectors);
                
                fluidDrawer.setup(&fluid);
            }
            
            
            float rfx = ofRandom(-randomForce, randomForce);
            float rfy = ofRandom(-randomForce, randomForce);
            fluid.addForceAtPos(Vec2f(ofRandomuf(), ofRandomuf()), Vec2f(rfx * rfx * rfx, rfy * rfy * rfy));
            
            fluid.update();
        }
        
        
        void GuiManager::injectVel(ofBaseHasPixels &maskImage, Vec2f *velArray, int velWidth, int velHeight) {
            if(!enabled) return;
            
            Vec2f avgVel;
            float vm = velMult * velMult * velMult;
            Vec2f velInvSize(1.0f/velWidth, 1.0f/velHeight);
            
            float mw = maskImage.getPixelsRef().getWidth();
            float mh = maskImage.getPixelsRef().getHeight();
            
            Vec2f velToMaskScaler = Vec2f(mw, mh) * velInvSize;
            
            float changedPixels = 0;
            Vec2f motionCenter;
            
            int velIndex = 0;
            
            unsigned char *maskPixels = maskImage.getPixels();
            for(int j=0; j<velHeight; j += fluidStep+1) {
                for(int i=0; i<velWidth; i += fluidStep+1) {
                    int maskIndex = (j * velToMaskScaler.y * mw) + (i * velToMaskScaler.x);
                    
                    if(maskPixels[maskIndex]>0) {
                        Vec2f vel = velArray[velIndex] * velInvSize;
                        avgVel += vel;
                        
                        Vec2f norm = Vec2f(i, j) * velInvSize;
                        
                        if(dyeAmount) {
                            float c = vel.lengthSquared() * dyeAmount;
                            fluid.addColorAtPos(norm, Color(c, c, c));
                        }
                        
                        vel *= vm;
                        
                        if(maxSpeed) vel.limit(maxSpeed * maxSpeed);
                        
                        fluid.addForceAtPos(norm, vel);
                        
                        changedPixels++;
                        motionCenter.x += i;
                        motionCenter.y += j;
                    }
                    
                    velIndex++;
                }
            }
            
            //		if(changedPixels>0) {
            //			stats.motionCenter.x = motionCenter.x / changedPixels * opticalFlow.getInvWidth();
            //			stats.motionCenter.y = motionCenter.y / changedPixels * opticalFlow.getInvHeight();
            //		}
            
            //			float lengthSquared = stats.avgVel.x * stats.avgVel.x + stats.avgVel.y * stats.avgVel.y;
            //			stats.avgSpeed2 = lengthSquared / particleEmitCounter * invWidth * invHeight;
            /// ((captureHeight/opticalFlow.captureRowsStep) * (opticalFlow.width/opticalFlow.captureColsStep));
        }
        
        void GuiManager::draw(float x, float y, float w, float h) {
            if(enabled && doDraw) {
                glColor3f(1, 1, 1);
                fluidDrawer.draw(x, y, w, h);
                fluidVectors.draw(x, y, w, h);
            }
        }
	}
}