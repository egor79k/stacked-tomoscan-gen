import os
import numpy as np
import tomophantom
from tomophantom import TomoP3D
from tomophantom.supp.artifacts import _Artifacts_
from tomobar.methodsDIR import RecToolsDIR
from PIL import Image
from config import *



h_det = int(np.sqrt(2) * size[0]) # detector column count (horizontal)
v_det = size[0] # detector row count (vertical)
angles_num = int(0.5 * np.pi * size[0]); # angles number
angles = np.linspace(0.0, 179.9, angles_num, dtype='float32') # in degrees
# angles = np.linspace(0, 180, 180, dtype='float32')
angles_rad = angles * (np.pi / 180.0)



def render_model():
    return TomoP3D.Model(model, size[0], models_lib)



def build_projections():
    return TomoP3D.ModelSino(model, size[0], h_det, v_det, angles, models_lib)



def split(data):
    partition = []

    if parts_num > 1:
        scan_height = int(size[0] / parts_num)
        half_overlay = int(overlay / 2)
        partition.append((0, scan_height + overlay))

        for scan_id in range(1, parts_num - 1):
            offset = scan_id * scan_height
            partition.append((offset - half_overlay, offset + scan_height + half_overlay))

        partition.append((size[0] - scan_height - overlay, size[0]))

    else:
        partition.append((0, size[0]))

    splitted_recs = []

    for part in partition:
        splitted_recs.append(data[part[0]:part[1],:,:])

    return splitted_recs



def apply_noise(projections_data):
    for projection in projections_data:
        print(len(projection), len(projection[0]), len(projection[0,0]))
        projection = _Artifacts_(projection, **noise_params)

    return projections_data



def reconstruct(projections_data):
    for projection in projections_data:
        RectoolsDIR = RecToolsDIR(DetectorsDimH = h_det,
                                DetectorsDimV = len(projection),
                                CenterRotOffset = None,
                                AnglesVec = angles_rad,
                                ObjSize = len(projection),
                                device_projector = 'cpu')

        projection = RectoolsDIR.FBP(projection) # FBP reconstruction

    return projections_data



def shrink_to_size(recs):
    for rec in recs:
        ld = int((size[0] - size[1]) / 2)
        rd = int(size[1] + ld)
        lw = int((size[0] - size[2]) / 2)
        rw = int(size[2] + lw)
        rec = rec[:,ld:rd,lw:rw]

    return recs



def apply_offset(recs):
    offsets = np.random.randint(-max_offset, max_offset, (parts_num, 2))

    for id, offset in enumerate(offsets):
        ld = (size[0] - size[1]) / 2 + offset[0]
        rd = size[1] + ld + offset[0]
        lw = (size[0] - size[2]) / 2 + offset[1]
        rw = size[2] + lw + offset[1]
        recs[id] = recs[id][:,ld:rd,lw:rw]

    return recs



def save_reconstructions(recs):
    os.makedirs(save_path, exist_ok=True)

    for rec_id, rec in enumerate(recs):
        rec_save_path = save_path + '/' + str(rec_id) + '/'
        os.mkdir(rec_save_path)

        for slice_id in range(len(rec)):
            img = Image.fromarray(np.array(rec[slice_id,:,:], dtype='float32'))
            img.save(rec_save_path + str(slice_id) + '.tiff', format='TIFF')




recs = []

if (is_noisy):
    print('Building 3D analytical projection data')
    proj = build_projections()

    print('Building partition')
    projs = split(proj)

    print('Adding noise to projection data')
    projs = apply_noise(projs)

    print('Reconstructing using FBP from tomobar')
    recs = reconstruct(projs)
else:
    print('Generate a size x size x size phantom')
    data = render_model()

    print('Building partition')
    recs = split(data)


if (is_offset):
    print('Shifting parts')
    recs = apply_offset(recs)
else:
    print('Shrinking to size')
    recs = shrink_to_size(recs)

print('Saving reconstructions')
save_reconstructions(recs)
