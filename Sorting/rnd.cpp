//
//  rnd.c
//  test1
//
//  Created by Oleg Bakharev on 06/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "rnd.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>

void randomInit()
{
    srand((unsigned)time(0));
//    srand(0);
}

int random( int i, int j )
{
    assert(j > i);
    return rand() % (j - i) + i;
}
