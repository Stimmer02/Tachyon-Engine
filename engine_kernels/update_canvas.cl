void kernel update_canvas(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private int2 coord = (int2)(get_global_id(0), get_global_id(1));
    private uchar tempID = resources->voxels[global_ID].substanceID;

    if (resources->collisionMap[global_ID] > 0){
        if (coord.x > 0 && coord.x < get_global_size(0)-1 && coord.y > 0 && coord.y < get_global_size(1)-1){
            write_imagef(TBO, (int2)(coord.x+1, coord.y-1), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x+1, coord.y), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x+1, coord.y+1), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x, coord.y-1), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x, coord.y+1), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x-1, coord.y-1), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x-1, coord.y), (float4)(1, 0, 0, 1.0f));
            write_imagef(TBO, (int2)(coord.x-1, coord.y+1), (float4)(1, 0, 0, 1.0f));
        }
        write_imagef(TBO, coord, (float4)(1, 1, 1, 1.0f));
    } else {
        write_imagef(TBO, coord, (float4)(0, 0.1f, 0, 1.0f));
    }
    
    // private int hash = resources->hashMap[global_ID];
    // private float x = (float)(hash % get_global_size(0)) / get_global_size(0);
    // private float y = (float)(hash / get_global_size(0)) / get_global_size(1);
    // write_imagef(TBO, coord, (float4)(x, y, 0.0f, 1.0f));
    // write_imagef(TBO, coord, *((global float4*)(&resources->SUBSTANCES[tempID].color)));
}
