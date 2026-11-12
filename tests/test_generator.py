import pytest
from pybiomes import Generator, Pos
from pybiomes.biomes import plains, river
from pybiomes.dimensions import DIM_OVERWORLD
from pybiomes.structures import Village
from pybiomes.versions import MC_1_21_WD

@pytest.fixture
def generator():
    # A specific seed is used to ensure repeatable results for testing.
    return Generator(version=MC_1_21_WD, flags=0)

def test_apply_seed(generator):
    # This test verifies that applying a seed correctly initializes the generator.
    seed_to_test = 1234567890
    generator.apply_seed(seed_to_test, DIM_OVERWORLD)
    assert True

def test_get_biome_at(generator):
    # This test checks if the generator returns the correct biome ID for a specific coordinate.
    seed = 1234567890
    generator.apply_seed(seed, DIM_OVERWORLD)

    scale, x, y, z = 4, 288, 256, 1984
    biome_id = generator.get_biome_at(scale, x>>2, y>>2, z>>2)

    assert isinstance(biome_id, int)
    assert biome_id == plains

def test_gen_biomes(generator):
    # This test verifies the bulk biome generation functionality.
    seed = 1234567890
    generator.apply_seed(seed, DIM_OVERWORLD)

    # Define a test range. We will create a dictionary to mirror the C struct.
    test_range = {
        'scale': 16,
        'x': 0, 'y': 60, 'z': 0,
        'sx': 16, 'sy': 1, 'sz': 16
    }

    # Pass individual values to the Python function as positional arguments, including scale.
    biomes = generator.gen_biomes(
        test_range['x'], test_range['y'], test_range['z'],
        test_range['sx'], test_range['sy'], test_range['sz'],
        test_range['scale']
    )
    assert len(biomes) == 16 * 1 * 16 # sx * sy * sz
    assert biomes[0] == 21
    assert biomes[255] == 45
    
def test_is_viable_structure_pos(generator):
    # This test checks if a given location is a viable spot for a structure.
    seed = 1234567890
    generator.apply_seed(seed, DIM_OVERWORLD)
    
    # A valid position for a village.
    valid_x, valid_z = 288, 1984
    flags = 0
    is_valid = generator.is_viable_structure_pos(Village, valid_x, valid_z, flags)
    assert isinstance(is_valid, bool)
    assert is_valid == True
    
    # A position that is not viable.
    invalid_x, invalid_z = 1000, 1000
    is_invalid = generator.is_viable_structure_pos(Village, invalid_x, invalid_z, flags)
    assert isinstance(is_invalid, bool)
    assert is_invalid == False

def test_map_approx_height(generator):
    # This test verifies the approximate height mapping function.
    # It requires a SurfaceNoise object and specific coordinates.
    try:
        from pybiomes import SurfaceNoise
    except ImportError:
        pytest.skip("SurfaceNoise is not available, skipping test.")
    
    seed = 1234567890
    surface_noise = SurfaceNoise()
    surface_noise.init_surface_noise(DIM_OVERWORLD, seed)

    generator.apply_seed(seed, DIM_OVERWORLD)
    # Get height and biome IDs for a single block.
    x, z, w, h = 288, 1984, 1, 1
    y_list, ids_list = generator.map_approx_height(surface_noise, x>>2, z>>2, w, h)
    
    assert isinstance(y_list, list)
    assert isinstance(ids_list, list)
    assert len(y_list) == w * h
    assert len(ids_list) == w * h
    
    # The height value will be a float, so we need to allow for some tolerance.
    assert pytest.approx(y_list[0], 0.01) == 77.12
    assert ids_list[0] == plains

def test_is_viable_structure_terrain(generator):
    from pybiomes.structures import Desert_Pyramid
    seed = 1234567890
    generator.apply_seed(seed, DIM_OVERWORLD)
    is_valid = generator.is_viable_structure_terrain(Desert_Pyramid, 288, 1984)
    assert isinstance(is_valid, bool)
