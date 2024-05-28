void kernel update_canvas(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private int2 coord = (int2)(get_global_id(0), get_global_id(1));
    private uchar tempID = resources->voxels[global_ID].substanceID;

    private int hash = resources->hashMap[global_ID];
    private float x = (float)(hash % get_global_size(0)) / get_global_size(0);
    private float y = (float)(hash / get_global_size(0)) / get_global_size(1);

    write_imagef(TBO, coord, (float4)(x, y, 0.0f, 1.0f));
    // write_imagef(TBO, coord, *((global float4*)(&resources->SUBSTANCES[tempID].color)));
}
