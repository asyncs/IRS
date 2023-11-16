world: {}
worldTranslationRotation (world): { joint: transXYPhi, Q: "d(90 0 0 1)", gains: [1, 1], limits: [-10, 10, -10, 10, -6, 6], ctrl_limits: [1, 1, 1], ctrl_H: 10, base }

Include: <pr2_clean.g>

Edit base_footprint (worldTranslationRotation): { Q: [0, 0, .05] }

#alternative collision shapes
coll_base (base_footprint): { shape: ssBox, Q: [0, 0, 0.12], size: [0.7, 0.7, 0.36, 0.1], contact: -2, coll_pr2, color: [1.,1.,1.,.2] }
coll_torso (base_footprint): { shape: ssBox, Q: [-0.13, 0, 0.6], size: [0.45, 0.7, 1.1, 0.1], contact: -2, coll_pr2, color: [1.,1.,1.,.2] }
coll_arm_r (r_upper_arm_roll_joint): { shape: ssBox, Q: [0.221337, 0, 0], size: [0.55, 0.2, 0.2, 0.1], contact: -4, coll_pr2, color: [1.,1.,1.,.2] }
coll_wrist_r (r_forearm_roll_joint): { shape: ssBox, Q: [0.21, 0, 0, 0.999391, 0, 0.0348995, 0], size: [0.35, 0.14, 0.14, 0.07], contact: -2, coll_pr2, color: [1.,1.,1.,.2] }
coll_hand_r (r_wrist_roll_joint): { shape: ssBox, Q: [0.12, 0, 0], size: [0.16, 0.12, 0.06, 0.025], contact: -2, coll_pr2, color: [1.,1.,1.,.2] }
coll_arm_l (l_upper_arm_roll_joint): { shape: ssBox, Q: [0.221337, 0, 0], size: [0.55, 0.2, 0.2, 0.1], contact: -4, coll_pr2, color: [1.,1.,1.,.2] }
coll_wrist_l (l_forearm_roll_joint): { shape: ssBox, Q: [0.21, 0, 0, 0.999391, 0, 0.0348995, 0], size: [0.35, 0.14, 0.14, 0.07], contact: -2, coll_pr2, color: [1.,1.,1.,.2] }
coll_hand_l (l_wrist_roll_joint): { shape: ssBox, Q: [0.12, 0, 0], size: [0.16, 0.12, 0.06, 0.025], contact: -2, coll_pr2, color: [1.,1.,1.,.2] }

Edit fl_caster_rotation_joint: { joint: none }
Edit fr_caster_rotation_joint: { joint: none }
Edit bl_caster_rotation_joint: { joint: none }
Edit br_caster_rotation_joint: { joint: none }
Edit fl_caster_l_wheel_joint: { joint: none }
Edit fl_caster_r_wheel_joint: { joint: none }
Edit fr_caster_l_wheel_joint: { joint: none }
Edit fr_caster_r_wheel_joint: { joint: none }
Edit bl_caster_l_wheel_joint: { joint: none }
Edit bl_caster_r_wheel_joint: { joint: none }
Edit br_caster_l_wheel_joint: { joint: none }
Edit br_caster_r_wheel_joint: { joint: none }

#Delete l_gripper_r_parallel_link_1: {}:
#Delete l_gripper_l_parallel_link_1: {}:
#Delete r_gripper_r_parallel_link_1: {}:
#Delete r_gripper_l_parallel_link_1: {}:

Edit r_gripper_l_finger_joint: { q: .1 }
Edit l_gripper_l_finger_joint: { q: .1 }
 
Include: <pr2_modifications.g>
