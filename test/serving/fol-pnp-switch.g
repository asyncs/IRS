QUIT
WAIT
INFEASIBLE
ANY
Terminate

FOL_World{
  hasWait=false
  gamma = 1.
  stepCost = 1.
  timeCost = 0.
}

## basic predicates
gripper
gripper_support
helper_gripper
object
table
jug
glass
waterSource
helper_zone
partOf


on
empty
filled
busy     # involved in an ongoing (durative) activity
free     # gripper hand is free
held     # object is held by an gripper
picked   # gripper X holds/has picked object Y
placed   # gripper X holds/has picked object Y
reached  # gripper X has reached object Y

## KOMO symbols
touch
above

touch
stable
stableOn

## initial state (generated by the code)
START_STATE {}

### RULES

#####################################################################

### Reward
REWARD {
}

#####################################################################

DecisionRule pick {
  X, Y
  { (gripper X) (object Y) (busy X)! (held Y)! } #(INFEASIBLE pick X Y)! }
  { (above Y ANY)! (on ANY Y)! (stableOn ANY Y)! 
    (picked X Y) (held Y) (busy X) # these are only on the logical side, to enable correct preconditions
    (touch X Y) (stable X Y) # these are predicates that enter the NLP
    }
}

#####################################################################

DecisionRule reach {
  X, Y
  { (gripper_support X) (object Y) (busy X)! (held Y)! } #(INFEASIBLE pick X Y)! }
  { (reached X Y) (busy X) # these are only on the logical side, to enable correct preconditions
    (touch X Y)# these are predicates that enter the NLP
    }
}

#####################################################################

DecisionRule place {
  X, Y, Z,
  { (picked X Y) (table Z) (held Y) }
  { (picked X Y)! (busy X)! (busy Y)! (held Y)! # logic only
    (stable ANY Y)! (touch X Y)! # NLP predicates
    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z)
    #(INFEASIBLE pick ANY Y)! block(INFEASIBLE pick ANY Y)
    }
}

#####################################################################

DecisionRule give {
  X, Y, Z,
  { (picked X Y) (helper_zone Z) (held Y) }
  { (picked X Y)! (busy X)! (busy Y)! (held Y)! # logic only
    (stable ANY Y)! (touch X Y)! # NLP predicates
    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z)
    #(INFEASIBLE pick ANY Y)! block(INFEASIBLE pick ANY Y)
    }
}

#####################################################################

DecisionRule fill {
  X, Y, Z,
  { (gripper X) (picked X Y) (waterSource Z) (glass Y) (held Y) (empty Y)}
  { (picked X Y)! (busy X)! (busy Y)! (held Y)! (empty Y)! (filled Y)# logic only
    (stable ANY Y)! (touch X Y)! # NLP predicates
    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z)
    #(INFEASIBLE pick ANY Y)! block(INFEASIBLE pick ANY Y)
    }
}

#####################################################################

DecisionRule carryTransportAffordable {
  X, Y, Z, O1,
  { (picked X Y) (table Z) (held Y) (on Y O1) }
  { (picked X Y)! (busy X)! (busy Y)! (held Y)! # logic only
    (stable ANY Y)! (touch X Y)! # NLP predicates
    (on Z Y) (above Y Z) (stableOn Z Y) tmp(touch X Y) tmp(touch Y Z) (on Z O1)
    }
}

#####################################################################