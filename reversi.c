#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define	EXPSPACE '.'

char	p[2] = { 'o', 'x' };

void
	printfield( char field[][8] )
{
	puts( "    A   B   C   D   E   F   G   H" );

	int	i, j;
	i = 0;
	while( i < 8 )
	{
		puts( "  +---+---+---+---+---+---+---+---+" );
		printf( "%d", i + 1 );
		j = 0;
		while( j < 8 )
		{
			fputs( " | ", stdout );
			putchar( field[i][j] );

			j++;
		}
		puts( " |" );

		i++;
	}
	puts( "  +---+---+---+---+---+---+---+---+" );
}

int
	isinside( int x, int y )
{
	return	0 <= x && x < 8 && 0 <= y && y < 8;
}

int
	ispiece( char field[][8], int x, int y )
{
	return	isinside( x, y ) && ( field[x][y] == 'o' || field[x][y] == 'x' );
}

int
	isntpiece( char field[][8], int x, int y )
{
	return	isinside( x, y ) && !( field[x][y] == 'o' || field[x][y] == 'x' );
}

int
	expect( char field[][8], int* turnptr, int score[] )
{
	int	expectnum = 0;
	*turnptr ^= 1;

	int	fine = 0;
	int	i, j;
	int	dx, dy;
	int	x, y;
	int	scale;

	score[0] = 0;
	score[1] = 0;
	i = 0;
	while( i < 8 )
	{
		j = 0;
		while( j < 8 )
		{
			switch( field[i][j] ){
			case	'o':
			{
				score[0]++;

				break;
			}
			case	'x':
			{
				score[1]++;

				break;
			}
			case	EXPSPACE:
			{
				fine = 1;
				field[i][j] = ' ';

				break;
			}}

			j++;
		}

		i++;
	}

	i = 0;
	while( i < 8 )
	{
		j = 0;
		while( j < 8 )
		{
			if( field[i][j] == p[*turnptr ^ 1] )
			{
				dx = -1;
				while( dx <= 1 )
				{
					x = i + dx;
					dy = -1;
					while( dy <= 1 )
					{
						y = j + dy;

						if( isntpiece( field, i - dx, j - dy ) )
						{
							scale = 1;
							x = i + scale * dx;
							y = j + scale * dy;
							while( isinside( x, y ) && field[x][y] == p[*turnptr ^ 1] )
							{
								scale++;

								x = i + scale * dx;
								y = j + scale * dy;
							}

							if( isinside( x, y ) && field[x][y] == p[*turnptr] )
							{
								if( field[i - dx][j - dy] == ' ' )
								{
									expectnum++;
								}

								field[i - dx][j - dy] = EXPSPACE;
							}

						}

						dy++;
					}

					dx++;
				}
			}

			j++;
		}

		i++;
	}

	if( expectnum == 0 && fine == 1 )
	{
		printf( "\n\n%c cannot put piece.\n", p[*turnptr] );

		return	expect( field, turnptr, score );
	}

	return	expectnum;
}

int
	putpiece( char field[][8], int i, int j, int turn )
{
	if( field[i][j] != EXPSPACE )
	{
		return	-1;
	}

	field[i][j] = p[turn];

	int	x, y;
	int	dx, dy;
	int	scale;

	dx = -1;
	while( dx <= 1 )
	{
		dy = -1;
		while( dy <= 1 )
		{
			scale = 1;
			x = i + dx;
			y = j + dy;
			while( isinside( x, y ) && field[x][y] == p[turn ^ 1] )
			{
				scale++;

				x = i + scale * dx;
				y = j + scale * dy;
			}

			if( isinside( x, y ) && field[x][y] == p[turn] )
			{
				while( scale >= 1 )
				{
					scale--;

					x = i + scale * dx;
					y = j + scale * dy;

					field[x][y] = p[turn];
				}
			}


			dy++;
		}

		dx++;
	}

	return	0;
}


void
	printresult( int score[] )
{
	printf( " o :  x\n%2d : %2d\n\n", score[0], score[1] );
	if( score[0] > score[1] )
	{
		puts( "o wins!" );
	}
	else if( score[0] > score[1] )
	{
		puts( "Draw!" );
	}
	else if( score[0] < score[1] )
	{
		puts( "x wins!" );
	}
}

int
	main()
{
	int	turn = 1;
	char	field[8][8] =
			{
				{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', 'x', 'o', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', 'o', 'x', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
				{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
			};
	int	score[2];
	int	x, y;

	
	while( expect( field, &turn, score ) )
	{
		printfield( field );

		do
		{
			do
			{
				printf( "[%c]: ", p[turn] );
				y = getc( stdin );
				x = getc( stdin );
				while( getchar() != '\n' );

				if( x == 'x' && y == 'x' )
				{
					exit( -1 );
				}

				x -= '1';
				y = toupper( y ) - 'A';
			}
			while( !isinside( x, y ) && ( puts( "Input correct value." ), 1 ) );
		}
		while( putpiece( field, x, y, turn ) < 0 && ( puts( "Select correct space." ), 1 ) );
	}
	printfield( field );
	printresult( score );

	return	0;
}
