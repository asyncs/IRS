frame floorwalls: { }
frame _1(floorwalls): { shape: box, size: [8, 6, 0.01], color: [0.82, 0.7, 0.55], Q: [1, 0, -0.005, 1, 0, 0, 0] }
frame _2(floorwalls): { shape: box, size: [8, 0.02, 2], color: [0.82, 0.7, 0.55], Q: [1, -3, 1, 1, 0, 0, 0] }
frame _3(floorwalls): { shape: box, size: [8, 0.02, 2], color: [0.82, 0.7, 0.55], Q: [1, 3, 1, 1, 0, 0, 0] }
frame _4(floorwalls): { shape: box, size: [0.02, 6, 2], color: [0.82, 0.7, 0.55], Q: [5, 0, 1, 1, 0, 0, 0] }
frame _5(floorwalls): { shape: box, size: [0.02, 6, 2], color: [0.82, 0.7, 0.55], Q: [-3, 0, 1, 1, 0, 0, 0] }

frame kitchen_counters: { }
frame _6(kitchen_counters): { shape: ssBox, X: "t(-2.6 0.5 .6) d(0 90 0 1)", size: [.8, 1, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }
frame _7(kitchen_counters): { shape: ssBox, X: "t(-2.6 2.5 .6) d(0 90 0 1)", size: [.8, 1, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }
frame _8(kitchen_counters): { shape: ssBox, X: "t(-2 -1.4 .6) d(90 0 0 1)", size: [.8, 2, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }


frame sink: { shape: ssBox, X: "t(-2.6 -0.5 .6) d(0 90 0 1)", size: [.8, 1, .1, .02], color: [0.25, 0.25, 0.75], fixed, contact, logical: { } }
frame stove: { shape: ssBox, X: "t(-2.6 1.5 .6) d(0 90 0 1)", size: [.8, 1, .1, .02], color: [0.7, 0.25, 0.25], fixed, contact, logical: { } }

frame kitchen_table: { shape: ssBox, X: "t(-1 1.5 .6) d(0 90 0 1)", size: [.8, 3, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }

shelf: { X: "t(-2.7 -2.4 0) d(0 90 0 1)" fixed }
(shelf): { shape: ssBox, color: [.62, .37, .12], Q: "T t(0 .55 .55)", size: [.6, .1, 1.05, .02], contact }
(shelf): { shape: ssBox, color: [.62, .37, .12], Q: "T t(0 -.55 .55)", size: [.6, .1, 1.05, .02], contact }
(shelf): { shape: ssBox, color: [.62, .37, .12], Q: "T t(0 0 .5)", size: [.6, 1., .1, .02], contact }
(shelf): { shape: ssBox, color: [.62, .37, .12], Q: "T t(0 0 1.)", size: [.6, 1., .1, .02], contact }

frame dining_table: { shape: ssBox, X: "t(3.5 -2 .6) d(90 0 0 1)", size: [1, 2, .1, .02], color: [.62, .37, .12], fixed, contact, logical: { } }

frame kitchen_counters: { }
frame _6(kitchen_counters): { shape: ssBox, X: "t(-2.6 0.5 .6) d(0 90 0 1)", size: [.8, 1, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }
frame _7(kitchen_counters): { shape: ssBox, X: "t(-2.6 2.5 .6) d(0 90 0 1)", size: [.8, 1, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }
frame _8(kitchen_counters): { shape: ssBox, X: "t(-2 -1.4 .6) d(90 0 0 1)", size: [.8, 2, .1, .02], color: [.3, .3, .3], fixed, contact, logical: { } }

frame living_sofa: { shape: ssBox, X: "t(2 1 .15) d(90 0 0 1)", size: [.8, 2, .3, .02], color: [.94, .76, .41], fixed, contact, logical: { } }

frame coffee_table: { shape: ssBox, X: "t(2 2.2 .6) d(90 0 0 1)", size: [0.7, 1, .1, .02], color: [.62, .37, .12], fixed, contact, logical: { } }




