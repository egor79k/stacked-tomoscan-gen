import os
import numpy as np
import tomophantom
from tomophantom import TomoP3D
from tomophantom.supp.artifacts import _Artifacts_
from tomobar.methodsDIR import RecToolsDIR
from PIL import Image


# Select a model number from the library
model = 13

# Define phantom dimensions using a scalar value
size = 256

# Number of vertical scans
scans_num = 4

# Overlay between adjacent scans in slices
overlay = 8

# Apply noise
apply_noise = True

# Noise parameters
noise_params =  {'noise_type' : 'Poisson',
                'noise_amplitude' : 10000,
                'noise_seed' : 0}

# Path for saving slices
folder = "img/Shepp-Logan"


partition = []

print('Building partition')
if scans_num > 1:
    scan_height = int(size / scans_num)
    half_overlay = int(overlay / 2)
    partition.append((0, scan_height + overlay))

    for scan_id in range(1, scans_num - 1):
        offset = scan_id * scan_height
        partition.append((offset - half_overlay, offset + scan_height + half_overlay))

    partition.append((size - scan_height - overlay, size))

else:
    partition.append((0, size))


path = os.path.dirname(tomophantom.__file__)
path_library3D = os.path.join(path, "Phantom3DLibrary.dat")
rec_result = []


if apply_noise:
    h_det = int(np.sqrt(2) * size) # detector column count (horizontal)
    v_det = size # detector row count (vertical)
    angles_num = int(0.5*np.pi * size); # angles number
    angles = np.linspace(0.0,179.9,angles_num,dtype='float32') # in degrees
    # angles = np.linspace(0, 180, 180, dtype='float32')
    angles_rad = angles*(np.pi/180.0)

    print('Building 3D analytical projection data')
    projData3D_analyt= TomoP3D.ModelSino(model, size, h_det, v_det, angles, path_library3D)

    print('Adding noise to projection data')
    projData3D_analyt_noisy = _Artifacts_(projData3D_analyt, **noise_params)
    
    print('Reconstructing using FBP from tomobar')
    RectoolsDIR = RecToolsDIR(DetectorsDimH = h_det,
                            DetectorsDimV = v_det,
                            CenterRotOffset = None,
                            AnglesVec = angles_rad,
                            ObjSize = size,
                            device_projector = 'cpu')

    rec_result = RectoolsDIR.FBP(projData3D_analyt_noisy) # FBP reconstruction

else:
    print('Generate a size x size x size phantom')
    rec_result = TomoP3D.Model(model, size, path_library3D)


os.makedirs(folder, exist_ok=True)

print('Saving scans')
for part_id, part in enumerate(partition):
    part_folder = folder + '/' + str(part_id) + '/'
    os.mkdir(part_folder)

    for slice_id in range(part[0], part[1]):
        img = Image.fromarray(np.array(rec_result[slice_id,:,:] * 255, dtype=np.int8), mode='L')
        img.save(part_folder + str(slice_id) + '.tiff', format='TIFF')