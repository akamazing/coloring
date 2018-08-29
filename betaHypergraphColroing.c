#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define MAX_EDGES 4950
#define k_uniform 3

int max_no_of_colors = 2,beta = 1,e = MAX_EDGES,n = MAX_VERTICES, delta = 0;
time_t start_time, current_time;
int flag1 = 0,flag2 = 0,done = 0;
int colors[MAX_EDGES][k_uniform];

int compare (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}

typedef struct vertex
{
	int num;
	int color;
	int size;
	int edges_belongs_to[MAX_EDGES];

}vertex;

typedef struct edge
{
	int num;
	int vertices_in_edge[k_uniform];
	int colors_of_vertices[k_uniform];

}edge;

vertex vertices[MAX_VERTICES];
edge edges[MAX_EDGES];
int neighbours[MAX_EDGES][MAX_EDGES];
int same_coloring[MAX_EDGES][MAX_EDGES];
int copy_of_colors[MAX_EDGES][k_uniform];

void update_colors_matrix()
{
	int i,j;
	for ( i = 0; i < e; ++i)
	{
		edge tmp = edges[i];
		for ( j = 0; j < k_uniform; ++j)
		{
			vertex v = vertices[tmp.vertices_in_edge[j]];
			edges[i].colors_of_vertices[j] = v.color;
			colors[i][j] = v.color;
		}
	}
}

void check_type1_error()
{
	// the edge is monochromatic
	flag1 = 0;
	int i,j,flag = 0;
	for ( i = 0; i < e; ++i)
	{
		flag = 0;
		edge tmp = edges[i];
		int first_color = vertices[tmp.vertices_in_edge[0]].color;
		for ( j = 1; j < k_uniform; ++j)
		{
			if( vertices[tmp.vertices_in_edge[j]].color != first_color )
			{
				// legal edge
				flag = 1;
			}
		}

		if(flag == 0)
		{
			// not a legal edge
			// printf("Type 1 error found for edge %d\n",i );
			int k = rand() % k_uniform; // choose a random vertex in that edge
			vertices[tmp.vertices_in_edge[k]].color = rand() % max_no_of_colors; // re-color it
			update_colors_matrix();
			return;
		}
	}

	// no type1 errors found !

	flag1 = 1;

}

void check_type2_error()
{
	flag2 = 0;
	// if more than beta neighbours have the same color scheme, then change extra ones
	int i,j,k,flag = 0,count;
	int color_of_edge[k_uniform];

	// Sorting the color scheme for each edge

	for( i = 0; i < e; i++ )
	{
		edge tmp = edges[i];
		for( j = 0; j < k_uniform; j++ )
		{
			color_of_edge[j] = tmp.colors_of_vertices[j];
		}

		qsort(color_of_edge,k_uniform,sizeof(int),compare);

		for( j = 0; j < k_uniform; j++ )
		{
			copy_of_colors[i][j] = color_of_edge[j];
		}

	}

	for( i = 0; i < e; i++ )
	{
		for( j = 0; j < e; j++ )
		{
			same_coloring[i][j] = 0;
		}
	}

	for( i = 0; i < e; i++ )
	{
		for( j = i+1; j < e; j++ )
		{
			// check if they(edge j and i) have the same color scheme
			flag = 0;
			for ( k = 0; k < k_uniform; ++k)
			{
				if( copy_of_colors[i][k] != copy_of_colors[j][k] )
				{
					flag = 1;
				}
			}

			if (flag == 0)
			{
				// edge i and edge j have the same coloring
				same_coloring[i][j] = 1;
				same_coloring[j][i] = 1;
			}

		}
	}


	flag = 0;

	for( i = 0; i < e; i++ )
	{
		for( j = 0; j < e; j++ )
		{
			if( neighbours[i][j] == 1 )
			{
				// j : a neighbour  of edge i
				count = 0;
				for ( k = 0; k < e; ++k)
				{
					if( neighbours[i][k] == 1)
					{
						// k : a neighbour  of edge i
						if( same_coloring[j][k] == 1 )
							count++;
					}
				}

				edge tmp = edges[j];

				if(count >= beta)
				{
					// type 2 error
					// printf("\t*******Type 2 error found for edge %d **********\n",i );
					flag = 1;
					int m = rand() % k_uniform; // choose a random vertex in that edge
					vertices[tmp.vertices_in_edge[m]].color = rand() % max_no_of_colors; // re-color it
					update_colors_matrix();
					return;

				}

			}
		}
	}

	//no type2 error found

	if(flag == 0)
		flag2 = 1;
}

void display_coloring()
{
	int i,j;
	printf("\n\ncolors Assigned : \n\n");
	for ( i = 0; i < n; ++i)
	{
		printf("Vertex %d - Color %d\n",i,vertices[i].color );
	}
	printf("\n\n");

	for ( i = 0; i < e; ++i)
	{
		edge tmp = edges[i];
		for ( j = 0; j < k_uniform; ++j)
		{
			printf(" %d (v%d) - ",tmp.colors_of_vertices[j],tmp.vertices_in_edge[j] );
		}
		printf("\n");
	}
	printf("\n\n");
}



int do_complete_checking()
{
	current_time = time(NULL);
  	while( (current_time - start_time) < 10 && flag1 != 1 && flag2 != 1 ) // Can change value here
  	{
  		check_type1_error();
  		if( flag1 == 0 ) // type1 errors still exist
  		{
  			current_time = time(NULL);
  			continue;
  		}

  		// no type1 errors
  		check_type2_error();
  		if( flag2 == 0 ) // type1 errors still exist
  		{
  			flag1 = 0;
  			flag2 = 0;
  			current_time = time(NULL);
  			continue;
  		}
  	}

  	if( flag1 == 1 && flag2 == 1 )
  	{
  		// Sucess
  		printf("\n\tSucessfull coloring found with %d colors\n",max_no_of_colors );
  		display_coloring();
  		return 1;
  	}
  	else
  	{
  		printf("\nTimed Out ! \n\t No coloring found with %d colors\n",max_no_of_colors );
  		return 0;
  	}
}

void color_whole_graph()
{
	int i,k;
	for ( i = 0; i < n; ++i)
	{
		k = rand() % max_no_of_colors;
		vertices[i].color = k;
	}
	update_colors_matrix();
}

int main(int argc, char const *argv[])
{
	int i,j,k,l;
	do
    {
        printf("Enter no. of vertices (<= %d) : ",MAX_VERTICES);
        scanf("%d",&n);  //total vertices
    }
    while(n > MAX_VERTICES);

    do
    {
        printf("Enter no. of edges : ");
        scanf("%d",&e);  //total edges
    }
    while(e > n*(n-1)/2 );

    do
    {
        printf("Enter Value of Beta : ");
        scanf("%d",&beta);  // beta
    }
    while( beta < 1 );

    // coloring the vertices

    color_whole_graph();

    // Creating the graph

    int chosen[n];
	srand(time(NULL));
    for ( i = 0; i < e; ++i)
    {
    	// k = number of vertices in edge i
    	// k = (rand() % n)+1;
    	// edges[i].size = k;
    	edges[i].num = i;
    	for ( j = 0; j < n; ++j)
    	{
    		chosen[j] = 0;
    	}
    	// choose k vertices to be part of edge i
    	for( j = 0; j < k_uniform;)
    	{
    		l = rand()%n;
    		if(chosen[l] == 0)
    		{
    			chosen[l] = 1;
    			edges[i].vertices_in_edge[j] = l; //l = vertex number
    			edges[i].colors_of_vertices[j] = vertices[l].color;
    			int m = vertices[l].size;
    			vertices[l].edges_belongs_to[m] = i;
    			vertices[l].size = m+1;
    			j++;
    		}
    	}
 	}

 	//assigning neighbours

 	for ( i = 0; i < n; ++i)
 	{
 		for( j = 0; j < vertices[i].size; j++ )
 		{
 			for( k = j+1; k < vertices[i].size; k++ )
 			{
 				neighbours[vertices[i].edges_belongs_to[j]][vertices[i].edges_belongs_to[k]] = 1;
 				neighbours[vertices[i].edges_belongs_to[k]][vertices[i].edges_belongs_to[j]] = 1;
 			}
 		}
 	}

 	printf("\nEdge Matrix : \n");
 	// displaying edge matrix
 	for ( i = 0; i < e; ++i)
 	{
 		printf("Edge number - %d : ",i );
 		for ( j = 0; j < k_uniform; ++j)
 		{
 			printf("%d ",edges[i].vertices_in_edge[j] );
 		}
 		printf("\n");
 	}

 	printf("\nNeighbour Matrix : \n");
 	// displaying neighbours matrix
 	for ( i = 0; i < e; ++i)
 	{
 		for ( j = 0; j < e; ++j)
 		{
 			printf("%d ",neighbours[i][j] );
 		}
 		printf("\n");
 	}

 	for ( i = 0; i < n; ++i)
    {
    	vertices[i].num = i;
    	if(vertices[i].size > delta)
    		delta = vertices[i].size;
    }

 	// Assigning colors randomly !

 	// printf("Initial Coloring : \n");
 	// display_coloring();

 	update_colors_matrix();

 	while( done == 0 )
    {
	    // color_whole_graph();
	    printf("Initial Coloring : \n");
 		display_coloring();
	    start_time = time(NULL);
	  	current_time = time(NULL);
	    done = do_complete_checking();
	    if( done == 0 )
	    {
	    	max_no_of_colors++;
	    	color_whole_graph();
	    }
	    else
	    {
	    	printf("Same coloring matrix : \n");
			for ( i = 0; i < e; ++i)
			{
				for ( j = 0; j < e; ++j)
				{
					printf("%d ",same_coloring[i][j] );
				}
				printf("\n");
			}

			printf("\nSorted color matrix : \n");
			for ( i = 0; i < e; ++i)
			{
				for ( j = 0; j < k_uniform; ++j)
				{
					printf("%d ",copy_of_colors[i][j] );
				}
				printf("\n");
			}
	    }
	    current_time = time(NULL);
	    printf("Time taken : %ld seconds\n",current_time - start_time );
	}

	return 0;
}
