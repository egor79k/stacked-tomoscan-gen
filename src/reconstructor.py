import os
import sys
import numpy as np
from tomobar.methodsDIR import RecToolsDIR
from PIL import Image


'''
This script reconstructs parts of the whole object from partial projections, using ToMoBAR reconstruction tools
Input:
    <read_path> <parts_count> <angles_count> <angles_step> <save_path>
    where:
        read_path    - path to projections dir
        parts_count  - number of dirs (named 0/, 1/, 2/, ...) in read_path
        angles_count - number of projection images (named 0.tiff, 1.tiff, ...) in each part dir
        angles_step  - step between adjacent projections
        save_path    - path to reconstructions save dir
Output:
    reconstructed part as a set of slices images at each of parts_count dirs at save_path
'''


def read_projections(part_id: int):
    part_read_path = os.path.join(read_path, str(part_id))
    with Image.open(os.path.join(part_read_path, str(part_id) + '.tiff')) as img:
        projs = np.empty((img.size[1], angles_num, img.size[0]), dtype='float32')

    for proj_id in range(angles_num):
        img = Image.open(os.path.join(part_read_path, str(proj_id) + '.tiff'))
        projs[:, proj_id, :] = np.array(img)

    return projs


def reconstruct(projs):
    RectoolsDIR = RecToolsDIR(DetectorsDimH = projs.shape[2],
                            DetectorsDimV = projs.shape[0],
                            CenterRotOffset = None,
                            AnglesVec = angles_rad,
                            ObjSize = projs.shape[2],
                            device_projector = 'cpu')

    return RectoolsDIR.FBP(projs)


def save_reconstructions(rec, part_id: int):
    rec_save_path = os.path.join(save_path, str(part_id))
    os.makedirs(rec_save_path, exist_ok=True)

    for slice_id in range(len(rec)):
        img = Image.fromarray(rec[slice_id,:,:])
        img.save(os.path.join(rec_save_path, str(slice_id) + '.tiff'), format='TIFF')


if len(sys.argv) < 6:
    print('Error: Not enougth args\nUsage: <read_path> <parts_count> <angles_count> <angles_step> <save_path>')
    sys.exit()

read_path = sys.argv[1]
parts_num = int(sys.argv[2])
angles_num = int(sys.argv[3])
angles_step = float(sys.argv[4])
save_path = sys.argv[5]

angles = np.arange(0, angles_num, angles_step)
angles_rad = angles * (np.pi / 180.0)

for part_id in range(parts_num):
    print('Part ', part_id, '\n- Reading projections')
    projs = read_projections(part_id)
    print('- Reconstructing')
    rec = reconstruct(projs)
    print('- Saving reconstructions')
    save_reconstructions(rec, part_id)