void kernel update_canvas(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private int2 coord = (int2)(get_global_id(0), get_global_id(1));
    private uchar tempID = resources->voxels[global_ID].substanceID;

    write_imagef(TBO, coord, *((global float4*)(&resources->SUBSTANCES[tempID].color)));
}