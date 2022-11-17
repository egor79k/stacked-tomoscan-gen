import os
import sys
import json
import numpy as np
from tomobar.methodsDIR import RecToolsDIR
import cv2

'''
This script reconstructs parts of the whole object from partial projections, using ToMoBAR reconstruction tools
Input:
    <read_path> <obj_size> <width> <parts_count> <angles_count> <angles_step> <save_path>
    where:
        read_path    - path to projections dir
        obj_size     - size of object used in projections building
        width        - side size
        parts_count  - number of dirs (named 0/, 1/, 2/, ...) in read_path
        angles_count - number of projection images (named 0.tiff, 1.tiff, ...) in each part dir
        angles_step  - step between adjacent projections
        save_path    - path to reconstructions save dir
Output:
    reconstructed part as a set of slices images at each of parts_count dirs at save_path
'''


def read_projections(part_id: int):
    part_read_path = os.path.join(read_path, str(part_id))
    size = cv2.imread(os.path.join(part_read_path, str(part_id) + '.tiff'), cv2.IMREAD_UNCHANGED).shape
    projs = np.empty((size[0], angles_num, size[1]), dtype='float32')

    for proj_id in range(angles_num):
        projs[:, proj_id, :] = cv2.imread(os.path.join(part_read_path, str(proj_id) + '.tiff'), cv2.IMREAD_UNCHANGED)

    return projs


def reconstruct(projs):
    RectoolsDIR = RecToolsDIR(DetectorsDimH = projs.shape[2],
                            DetectorsDimV = projs.shape[0],
                            CenterRotOffset = None,
                            AnglesVec = angles_rad,
                            ObjSize = obj_size,
                            device_projector = 'cpu')

    lw = int((obj_size - width) / 2)
    rw = int(obj_size - lw)
    
    return RectoolsDIR.FBP(projs)[:, lw:rw, lw:rw]


def save_reconstructions(rec, part_id: int):
    rec_save_path = os.path.join(save_path, str(part_id))
    os.makedirs(rec_save_path, exist_ok=True)

    # Write images
    for slice_id in range(len(rec)):
        cv2.imwrite(os.path.join(rec_save_path, str(slice_id) + '.tiff'), rec[slice_id,:,:]);

    # Write parameters
    params = {
        'height': len(rec),
        'depth': len(rec[0]),
        'width': len(rec[0, 0]),
        'format': '.tiff',
        'range_min': float(np.min(rec)),
        'range_max': float(np.max(rec))
    }

    with open(os.path.join(rec_save_path, 'info.json'), 'w') as fp:
        json_params = json.dump(params, fp)


if len(sys.argv) < 8:
    print('Error: Not enougth args\nUsage: <read_path> <obj_size> <parts_count> <angles_count> <angles_step> <save_path>')
    sys.exit()

read_path = sys.argv[1]
obj_size = int(sys.argv[2])
width = int(sys.argv[3])
parts_num = int(sys.argv[4])
angles_num = int(sys.argv[5])
angles_step = float(sys.argv[6])
save_path = sys.argv[7]

angles = np.arange(0, angles_num * angles_step, angles_step)
angles_rad = angles * (np.pi / 180.0)

for part_id in range(parts_num):
    print('Part ', part_id, '\n- Reading projections')
    projs = read_projections(part_id)
    print('- Reconstructing ', projs.shape)
    rec = reconstruct(projs)
    print('- Saving reconstructions')
    save_reconstructions(rec, part_id)

# Write parameters
params = {
    'parts_num': parts_num
}

with open(os.path.join(save_path, 'info.json'), 'w') as fp:
    json_params = json.dump(params, fp)
