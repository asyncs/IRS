Include: <../panda/panda.g>
Include: <../pr2/pr2.g>

Edit panda_link0{ X: "t(2.5 0 .65) d(180 0 0 1)" }
Edit panda_finger_joint1 { joint_active: False }
Edit base_footprint (worldTranslationRotation): { Q: [2, -2, .05] }



Frame table_panda: { shape: ssBox, X: "t(2 0 .6) d(-90 0 0 1)", size: [1, 2, .1, .02], color: [0, 0, 0], fixed, contact, logical: { helper_zone } }
Frame table_obs: { shape: ssBox, X: "t(0.5 0 .6) d(90 0 0 1)", size: [1, 0.9, .1, .02], color: [0, 0, 0], fixed, contact, logical: { } }
Frame table_south: { shape: ssBox, X: "t(0 2.5 .6) d(90 0 0 1)", size: [1, 1.5, .1, .02], color: [0, 0, 0], fixed, contact, logical: { } }
Frame table_east: { shape: ssBox, X: "t(-1.7 0 .6) d(90 0 0 1)", size: [1, 0.9, .1, .02], color: [0, 0, 0], fixed, contact, logical: { } }
Frame table_north: { shape: ssBox, X: "t(0 -2.5 .6) d(90 0 0 1)", size: [1, 1.5, .1, .02], color: [0, 0, 0], fixed, contact, logical: { } }
Frame goal_area: { shape: ssBox, X: "t(2.2 -1.1 .8) d(90 0 0 1)", size: [0.5, 0.5, .1, .02], color: [0, 0, 0], fixed, contact, logical: { table } }
Frame _: { shape: ssBox, X: "t(2.2 -1.1 .6) d(90 0 0 1)", size: [1, 0.9, .1, .02], color: [0, 0, 0], fixed, contact, logical: {  } }



Frame wall_1: { shape: ssBox, X: "t(3.1 0. .6)", size: [.1, 6, 2, .02], color: [0, 0, 0], fixed, contact, logical: {  } }
Frame wall_2: { shape: ssBox, X: "t(-2.5 0. .6)", size: [.1, 6, 2, .02], color: [0, 0, 0], fixed, contact, logical: {  } }








