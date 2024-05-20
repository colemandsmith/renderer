GIT_ROOT=$(git rev-parse --show-toplevel)
EX_ROOT=$GIT_ROOT/ExampleScenes

mkdir -p $EX_ROOT/models
mkdir -p $EX_ROOT/textures

# skybox
curl https://dl.polyhaven.org/file/ph-assets/HDRIs/hdr/2k/symmetrical_garden_02_2k.hdr --output $EX_ROOT/textures/skybox.hdr
