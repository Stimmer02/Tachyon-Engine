
            struct __attribute__ ((packed)) color{
                unsigned char R;
                unsigned char G;
                unsigned char B;
            };


            struct __attribute__ ((aligned)) vector2D{
                int x;
                int y;
            };

            struct __attribute__ ((aligned)) voxel{
                unsigned int substanceID;
                struct vector2D forceVector;
                // psisicStateVariables TODO next iteration
            };


            struct __attribute__ ((aligned)) substance{
            //     const char name;IMPOSIBLE
                struct color color;
                float mass;
                float jammingFactor;
                // psisicStateVariables TODO next iteration
            };

            struct  __attribute__ ((aligned))  substanceTable{
                __global struct substance* substances;
            };

            struct __attribute__ ((aligned)) engineConfig{
                unsigned int simulationWidth;
                unsigned int simulationHeight;
                float gravity;
                float airResistance;
                float timeFactor;
                // localDimensions TODO next iteration
            };

            struct __attribute__ ((aligned)) chunk{
                __global struct voxel* voxels;
            };


            struct __attribute__ ((aligned)) engineResources{
                __global struct substanceTable * substanceTable;
                __global struct chunk * worldMap;
                image2d_t * globalPBO;
            };

            void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){
                resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.x = 0;
                resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.y = 0;
                resources->worldMap->voxels[y * config->simulationWidth + x].substanceID = substanceID;
            }

            void kernel set_chunk(global struct chunk* matrix, global struct voxel* voxels){
                matrix->voxels = voxels;
            }
            void kernel set_substanceTable(global struct substanceTable* table, global struct substance* substances){
                table->substances = substances;
            }
            void kernel set_engineResources(global struct engineResources * resources, global struct substanceTable* table, global struct chunk* matrix){
               resources->substanceTable = table;
               resources->worldMap = matrix;
            }
            void kernel sum_voxel(global struct engineResources* resources, global uint* workArr, uint size, global uint* returnValue){
                private uint id = get_global_id(0);
                private uint dim = get_local_size(0);
                private char dividionRest = size & 0x1;
                private uint currentSize = size >> 1;
                private uint index;

                for (private uint i = id; i < currentSize; i += dim){
                   index = i << 1;
                    workArr[i] = (resources->worldMap->voxels[index].substanceID > 0) + (resources->worldMap->voxels[index+1].substanceID > 0);
                }
                barrier(CLK_LOCAL_MEM_FENCE);
                if (dividionRest){
                    if (id == 0){
                       workArr[currentSize] = (resources->worldMap->voxels[currentSize << 1].substanceID > 0);
                   }
                   currentSize++;
                }
                barrier(CLK_LOCAL_MEM_FENCE);
                dividionRest = currentSize & 0x1;
                currentSize >>= 1;

                while (currentSize > 1){
                    for (private uint i = id; i < currentSize; i += dim){
                       index = i << 1;
                       workArr[i] = workArr[index] + workArr[index+1];
                    }
                    barrier(CLK_LOCAL_MEM_FENCE);
                    if (dividionRest){
                       if (id == 0){
                           workArr[currentSize] = workArr[currentSize << 1];
                       }
                       currentSize++;
                   }
                    barrier(CLK_LOCAL_MEM_FENCE);
                    dividionRest = currentSize & 0x1;
                    currentSize >>= 1;
                }
                barrier(CLK_LOCAL_MEM_FENCE);
                if (id == 0){
                    *returnValue = workArr[0] + workArr[1];
                    if (dividionRest == 1){
                       *returnValue += workArr[2];
                    }
               }
           }
            void kernel spawn_voxel_in_area(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){
               uint globalID, IDX, IDY;
               IDX = x + get_global_id(0);
               IDY = y + get_global_id(1);
               globalID = IDY * config->simulationWidth + IDX;

                if (config->simulationWidth > IDX && config->simulationHeight > IDY){
                   resources->worldMap->voxels[globalID].forceVector.x = 0;
                   resources->worldMap->voxels[globalID].forceVector.y = 0;
                   resources->worldMap->voxels[globalID].substanceID = substanceID;
                }
           };
