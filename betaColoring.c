#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 100

int G[MAX][MAX]; // G:adjacency matrix
int max_no_of_colors = 20,beta = 1,e = 2000,n = MAX, delta = 0;
int no_of_neighbours[MAX], coloring[MAX];
time_t start_time, current_time;
int flag1 = 0,flag2 = 0,done = 0;

void display_coloring()
{
	int i,j;
	printf("\n-----------------------------------------------------\n");
    printf("Colours Assigned -->\n");
	for ( i = 0; i < n; ++i)
	{	 printf("\n Row %d(%d) : ",i,coloring[i]);
       		 for(j=0;j<n;j++)
      		  {
      		      if(G[i][j] == 1 && i != j)
				printf("%d(%d) ",j,coloring[j]);
      		  }
      		  printf("\n");
	}
}

void check_type1_error()
{
	// if two neighbours have the same color, then change either one
	int i,j,flag = 0;
	for ( i = 0; i < n; ++i)
	{
		for ( j = 0; j < n; ++j)
		{
			if( G[i][j] == 1 && coloring[i] == coloring[j] )
			{
				flag = 1;
				coloring[i] = rand() % max_no_of_colors;
			}
		}
	}

	// no type1 errors found !

	if( flag == 0 )
		flag1 = 1;

}

void check_type2_error()
{
	// if more than beta neighbours have the same color, then change extra ones
	int i,j,flag = 0;
	int no_of_neighbours_with_color[n];

	for ( i = 0; i < n; ++i)
	{
		no_of_neighbours_with_color[i] = 0;
	}

	for ( i = 0; i < n; ++i)
	{
		for ( j = 0; j < n; ++j)
		{
			no_of_neighbours_with_color[j] = 0;
		}

		for ( j = 0; j < n; ++j)
		{
			if( G[i][j] == 1 )
			{
				no_of_neighbours_with_color[coloring[j]]++;
				if( no_of_neighbours_with_color[coloring[j]] > beta )
				{
					coloring[j] = rand() % max_no_of_colors;
					flag = 1;
				}
			}
		}
	}

	//no type2 error found

	if(flag == 0)
		flag2 = 1;
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
		coloring[i] = k;
	}

}

void display ()
{
	int i,j;
	printf("\n-----------------------------------------------------\n");
	printf("No. of vertices = %d\n",n );
  	printf("No. of Edges = %d\n",e );
  	printf("Value of Beta = %d\n",beta );
  	printf("Value of Delta = %d\n",delta );
	printf("\n-----------------------------------------------------\n");
    printf("Adjacency Matrix -->\n");
    for(i=0;i<n;i++)
    {
        printf("\n Row %d : ",i);
        for(j=0;j<n;j++)
        {
            printf("%d, ",G[i][j]);
        }
        printf("\n");
    }
    printf("\n-----------------------------------------------------\n");
}

int main(int argc, char const *argv[])
{
	int i,j,k,l;
    do
    {
        printf("Enter no. of vertices (<= %d) : ",MAX);
        scanf("%d",&n);  //total vertices
    }
    while(n > MAX);

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

    /*for(i=0;i<n;i++)
    {
        x[i] = 0;
        for(j=0;j<n;j++)
        {
            if(i!=j)
                G[i][j]=1;  //assign 1 to all index of adjacency matrix
        }
    }*/

    for ( i = 0; i < n; ++i)
    {
    	no_of_neighbours[i] = 0;
    }

    srand(time(NULL));
    for(i=0;i<e;)
    {
        //scanf("%d %d",&k,&l);
        k = rand() % n;
        l = rand() % n;
        if( k != l && G[k][l] != 1 )
        {
            G[k][l] = 1;
            G[l][k] = 1;
            no_of_neighbours[l]++;
            no_of_neighbours[k]++;
            i++;
        }
    }

    for ( i = 0; i < n; ++i)
    {
    	if(no_of_neighbours[i] > delta)
    		delta = no_of_neighbours[i];
    }

    display();
    while( done == 0 )
    {
	    color_whole_graph();
	    start_time = time(NULL);
	  	current_time = time(NULL);
	    done = do_complete_checking();
	    if( done == 0 )
	    	max_no_of_colors++;
	    current_time = time(NULL);
	    printf("Time taken : %ld seconds\n",current_time - start_time );
	}

	return 0;
}
