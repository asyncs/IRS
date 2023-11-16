#!/usr/bin/python3

import os
import glob
import signal
from mesh_helper import *

files = sorted(glob.glob('*_description/meshes/**/*.stl'))

signal.signal(signal.SIGALRM, timeout)

for file in files:
    print('file: ', file)

    ### load
    mesh = load_mesh(file)
    if mesh==None:
        continue

    ### repair
    try:
        trimesh.constants.tol.merge = 1e-6
        mesh.process(validate=True)
        trimesh.repair.fill_holes(mesh)
        trimesh.repair.fix_inversion(mesh, multibody=True)
    except Exception as e:
        print('  --- repair failed ---', e)
        continue
    print('  watertight:', mesh.is_watertight)
    print('  oriented:', mesh.is_winding_consistent)

    ## export
    filebase = os.path.splitext(file)[0]
    print('  exporting:', filebase)

    export_mesh(mesh, filebase+'.mesh')
    try:
        mesh.export(filebase+'.ply')
    except:
        os.system('meshTool ' + filebase+'.mesh' + ' -hide -quiet'
              ' && mv z.ply ' + filebase + '.ply' )
        print('==== ply export failed ====')
    #test_read(filebase+'-.mesh')
