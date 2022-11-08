#==========================================================
# Model parameters
#----------------------------------------------------------
# Path to phantoms library
models_lib = 'models/Phantom3DLibrary.dat'

# Select a model number from the library
model = 13


#==========================================================
# Partition parameters
#----------------------------------------------------------
# Define phantom dimensions using a scalar value
size = (128, 128, 128)

# Number of vertical scans
parts_num = 2

# Overlay between adjacent scans in slices
overlay = 0


#==========================================================
# Images parameters
#----------------------------------------------------------
# Path for saving reconstructions
save_path = "img/Test"

# Image format
format = '.tiff'

# Pixels data type
type = 'float32'


#==========================================================
# Distortion parameters
#----------------------------------------------------------
# Seed for pseudorandom distortions
seed = 0

# Is need to apply noise
is_noisy = True

# Noise parameters
noise_params =  {'noise_type' : 'Poisson',
                'noise_amplitude' : 10000,
                'noise_seed' : seed}

# Is need to apply offsets
is_offset = False

# Max offset from axis of rotation in voxels
max_offset = 5
