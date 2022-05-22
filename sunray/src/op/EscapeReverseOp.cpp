// Ardumower Sunray 
// Copyright (c) 2013-2020 by Alexander Grau, Grau GmbH
// Licensed GPLv3 for open source use
// or Grau GmbH Commercial License for commercial use (http://grauonline.de/cms2/?page_id=153)

#include "op.h"
#include <Arduino.h>
#include "../../robot.h"
#include "../../StateEstimator.h"
#include "../../map.h"



String EscapeReverseOp::name(){
    return "EscapeReverse";
}

void EscapeReverseOp::begin(){
    // obstacle avoidance
    motor.setLinearAngularSpeed(-0.1,0);
    motor.setMowState(false);                                        
    driveReverseStopTime = millis() + 3000;                            
}


void EscapeReverseOp::end(){
}


void EscapeReverseOp::run(){
    battery.resetIdle();
    if (millis() > driveReverseStopTime){
        CONSOLE.println("driveReverseStopTime");
        motor.stopImmediately(false); 
        driveReverseStopTime = 0;
        if (detectLift()) {
            changeOp(errorOp);
            return;
        }
        if (maps.isDocking()){
            CONSOLE.println("continue docking");
            // continue without obstacles
            changeOp(*nextOp);    // continue current operation
        } else {
            CONSOLE.println("continue operation with virtual obstacle");
            maps.addObstacle(stateX, stateY);              
            //Point pt;
            //if (!maps.findObstacleSafeMowPoint(pt)){
            //    changeOp(dockOp); // dock if no more (valid) mowing points
            //} else changeOp(*nextOp);    // continue current operation
            changeOp(*nextOp);    // continue current operation
        }
    }
}


