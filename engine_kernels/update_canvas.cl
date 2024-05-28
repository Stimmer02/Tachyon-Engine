void kernel update_canvas(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID;
    private uchar tempID;
    private int2 coord = (int2)(get_global_id(0), get_global_id(1));

    tempID = resources->voxels[global_ID].substanceID;
    write_imagef(TBO, coord, *((global float4*)(&resources->SUBSTANCES[tempID].color)));

}
