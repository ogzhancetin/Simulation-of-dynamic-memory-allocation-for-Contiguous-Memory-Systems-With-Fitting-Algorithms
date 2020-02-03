
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pro_t {

	int id;
	int size;
	int address;

}process_t[10];

struct hol_t {

	int size;
	int address;

}holes_t[20];


int main(int args, char **argv) {


	int fixed_size_block = 4;

  int size_of_memory = atoi(argv[1]);
	int frames = size_of_memory / fixed_size_block;
	int frames_num = frames;

	int h = 0; // holes index
	int check = 0; // to determine is there any Ended process
	int flag;

	FILE *ptr = fopen(argv[2], "r");

	if (ptr == NULL)
		printf("Error occured while openning file\n");


	int fit_type = atoi(argv[3]);

	printf("Program Launched\n");

	int status, i = 0, id, memory, allocated, insuf = 0, ex_frag = 0, int_frag = 0;
	int lower_bound, upper_bound;

	char sit;


	int *array = (int*)malloc(sizeof(int)*frames);

	for (int z = 0; z < frames; z++)
	  array[z] = 0;

	int min = 999999, min_address, temp; // to best fit algorithm
	int max = 0, max_address; // to worst fit algorithm
	int address, size = 0;

	switch (fit_type){

	case 1:

		status = fscanf(ptr, "%c %d %d",&sit, &id, &memory);

		while (status != EOF) {

		  if (sit == 'E')
				allocated = process_t[id].size;

			else if (memory % fixed_size_block != 0)
				allocated = (memory / fixed_size_block) + 1;

			else
				allocated = memory / fixed_size_block;

			if (sit == 'B')
			{
				if (allocated <= frames)
				{
					flag = 1;
			  	upper_bound = 0;
					lower_bound = upper_bound + allocated;

					while (lower_bound < frames_num  && flag == 1)
					{
						flag = 0;
						for (int k = upper_bound; k < lower_bound; k++)
			  		{

							if (array[k] != 0)
							{
								flag = 1;
							}
						}
						upper_bound++;
						lower_bound++  ;
					}

					if (flag == 1) // external memory
					{

						printf("%c%5d%9d   -> %d frames will be used, ERROR! External fragmentation\n", sit, id, memory, allocated);
						ex_frag++;
					}

					else
				  {
						process_t[id].address = upper_bound - 1;
						frames -= allocated;
						printf("%c%5d%9d   -> %d frames will be used, remaining #frames: %d\n", sit, id, memory, allocated, frames);

						process_t[id].id = id;
						process_t[id].size = allocated;

						for (int j = upper_bound - 1; j < lower_bound - 1 ; j++)
							 array[j] = id;


						if (memory % 4 != 0)
						{
							int_frag += (4 - (memory % 4));
						}
					}
				}
				else
				{
					printf("%c%5d%9d   -> ERROR! Insufficient memory\n", sit, id, memory);
					insuf++;
          
          if (memory % 4 != 0)
	        {
         	  int_frag += (4 - (memory % 4));
          }

				}
			}
			if (sit == 'E')
			{
			  frames += allocated;

				printf("%c%5d%9c   -> %d frames are deallocated, available #frames: %d\n", sit, id, ' ', allocated, frames);

				for (int n = 0; n < frames_num; n++)
					if (array[n] == id)
						array[n] = 0;
			}

			status = fscanf(ptr, "%c", &sit);

			if (sit == 'B')
				status = fscanf(ptr, "%d %d", &id, &memory);
			if (sit == 'E')
				status = fscanf(ptr, "%d", &id);

		}

		break;

	case 2:


        status = fscanf(ptr, "%c %d %d", &sit, &id, &memory);


        while (status != EOF) {

            if (sit == 'E')
                allocated = process_t[id].size;

            else if (memory % fixed_size_block != 0)
                allocated = (memory / fixed_size_block) + 1;

            else
                allocated = memory / fixed_size_block;

            if (sit == 'B')
            {
                if (allocated <= frames)
                {
                    flag = 1;

                    upper_bound = 0;
                    lower_bound = upper_bound + allocated;

                    if (check != 1)
                    {
                        while (lower_bound < frames_num && flag == 1)
                        {
                            flag = 0;
                            for (int k = upper_bound; k < lower_bound; k++)
                            {

                                if (array[k] != 0)
                                {
                                    flag = 1;
                                }
                            }


                            upper_bound++;
                            lower_bound++;
                        }
                    }

                    else
                    {
                        int c_out = 0;

                        for (int u = 0; u < frames_num; u++)
                        {

                            if (array[u] == 0)
                            {
                                address = u;
                                while (array[u] == 0 && u < frames_num)
                                {
                                    size++;
                                    u++;
                                }
                                for (int b = 0; b < h; b++)
                                {
                                    if (holes_t[b].address == address)
                                         c_out = 1;
                                }
                                if (c_out == 0) {
                                    holes_t[h].size = size;
                                    holes_t[h].address = address;
                                    h++;
                                }

                            }
                            size = 0;
                        }
                        for (int a = 0; a < h; a++) {
                            if (holes_t[a].size >= allocated)
                            {
                                temp = holes_t[a].size - allocated;
                                if (temp < min && temp >= 0) {
                                    min = temp;
                                    min_address = holes_t[a].address;
                                    flag = 0;
                                }
                            }

                        }
                        if (flag == 0) {
                            upper_bound = min_address + 1;
                            lower_bound = upper_bound + allocated;
                        }
                        min = 999999;
                    }

                    if (flag == 1) // external memory
                    {

                        printf("%c%5d%9d   -> %d frames will be used, ERROR! External fragmentation\n", sit, id, memory, allocated);
                        ex_frag++;
                    }

                    else
                    {
                        process_t[id].address = upper_bound - 1;
                        frames -= allocated;
                        printf("%c%5d%9d   -> %d frames will be used, remaining #frames: %d\n", sit, id, memory, allocated, frames);

                        process_t[id].id = id;
                        process_t[id].size = allocated;
                        for (int a = 0; a < h; a++)
                        {
                            if (holes_t[a].address == process_t[id].address) {
                                holes_t[a].size = 0;
                                holes_t[a].address = -1;
                            }
                        }

                        for (int j = upper_bound - 1; j < lower_bound - 1; j++)
                            array[j] = id;

                        if (memory % 4 != 0)
                        {
                            int_frag += (4 - (memory % 4));
                        }
                    }
                }
                else
                {
                    printf("%c%5d%9d   ->ERROR! Insufficient memory\n", sit, id, memory);
                    insuf++;
                }
            }
            if (sit == 'E')
            {

                check = 1;
                frames += allocated;
                printf("%c%5d%9c   -> %d frames are deallocated, available #frames: %d\n", sit, id, ' ', allocated, frames);

                for (int n = 0; n < 256; n++)
                    if (array[n] == id)
                        array[n] = 0;


            }



            status = fscanf(ptr, "%c", &sit);

            if (sit == 'B')
                status = fscanf(ptr, "%d %d", &id, &memory);
            if (sit == 'E')
                status = fscanf(ptr, "%d", &id);




        }

        break;


	case 3:

        status = fscanf(ptr, "%c %d %d", &sit, &id, &memory);


        while (status != EOF) {

            if (sit == 'E')
                allocated = process_t[id].size;

            else if (memory % fixed_size_block != 0)
                allocated = (memory / fixed_size_block) + 1;

            else
                allocated = memory / fixed_size_block;

            if (sit == 'B')
            {
                if (allocated <= frames)
                {
                    flag = 1;

                    upper_bound = 0;
                    lower_bound = upper_bound + allocated;

                    if (check != 1)
                    {
                        while (lower_bound < frames_num && flag == 1)
                        {
                            flag = 0;
                            for (int k = upper_bound; k < lower_bound; k++)
                            {

                                if (array[k] != 0)
                                {
                                    flag = 1;
                                }
                            }


                            upper_bound++;
                            lower_bound++;
                        }
                    }

                    else
                    {
                        int c_out = 0;

                        for (int u = 0; u < frames_num; u++)
                        {

                            if (array[u] == 0)
                            {
                                address = u;
                                while (array[u] == 0 && u < frames_num)
                                {
                                    size++;
                                    u++;
                                }
                                for (int b = 0; b < h; b++)
                                {
                                    if (holes_t[b].address == address)
                                        c_out = 1;
                                }
                                if (c_out == 0) {
                                    holes_t[h].size = size;
                                    holes_t[h].address = address;
                                    h++;
                                }

                            }
                            size = 0;
                        }
                        for (int a = 0; a < h; a++) {
                            if (holes_t[a].size >= allocated)
                            {
                                temp = holes_t[a].size - allocated;
                                if (temp > max && temp >= 0) {
                                    max = temp;
                                    max_address = holes_t[a].address;
                                    flag = 0;
                                }
                            }

                        }
                        if (flag == 0) {
                            upper_bound = max_address + 1;
                            lower_bound = upper_bound + allocated;
                        }
                        max = 0;
                    }

                    if (flag == 1) // external memory
                    {

                        printf("%c%5d%9d   -> %d frames will be used, ERROR! External fragmentation\n", sit, id, memory, allocated);
                        ex_frag++;
                    }

                    else
                    {
                        process_t[id].address = upper_bound - 1;
                        frames -= allocated;
                        printf("%c%5d%9d   -> %d frames will be used, remaining #frames: %d\n", sit, id, memory, allocated, frames);

                        process_t[id].id = id;
                        process_t[id].size = allocated;
                        for (int a = 0; a < h; a++)
                        {
                            if (holes_t[a].address == process_t[id].address) {
                                holes_t[a].size = 0;
                                holes_t[a].address = -1;
                            }
                        }


                        for (int j = upper_bound - 1; j < lower_bound - 1; j++)
                            array[j] = id;

                        if (memory % 4 != 0)
                        {
                            int_frag += (4 - (memory % 4));
                        }
                    }
                }
                else
                {
                    printf("%c%5d%9d   ->ERROR! Insufficient memory\n", sit, id, memory);
                    insuf++;
                }
            }
            if (sit == 'E')
            {

                check = 1;
                frames += allocated;
                printf("%c%5d%9c   -> %d frames are deallocated, available #frames: %d\n", sit, id, ' ', allocated, frames);

                for (int n = 0; n < frames_num; n++)
                    if (array[n] == id)
                        array[n] = 0;


            }



            status = fscanf(ptr, "%c", &sit);

            if (sit == 'B')
                status = fscanf(ptr, "%d %d", &id, &memory);
            if (sit == 'E')
                status = fscanf(ptr, "%d", &id);




        }

        break;

	default:
		printf("No such a strategy number %d\n", fit_type);
		break;
	}

	printf("\nTotal free memory in holes: %d frames, %d KB\n", frames, frames * 4);
        printf("Total memory wasted as an internal fragmentation: %d KB\n", int_frag);
        printf("Total number of rejected processes due to external fragmentation : %d\n", ex_frag);
        printf("Total number of rejected processes due to insufficient memory: %d\n", insuf);
	printf("\nHoles:\n");

	int hole_addr, hole_size = 0;

	for(int i = 0 ; i < frames_num ; i++)
	{
		if(array[i] == 0)
		{
			hole_addr = i;
			while(array[i] == 0 && i < frames_num)
			{
			hole_size++;
			i++;
			}
		printf("%d %d\n",hole_addr,hole_size);

		}
	hole_size = 0;
	}

	return 0;
}
