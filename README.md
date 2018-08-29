# β Frugal coloring
The algorithm implemented here is a randomized algorithm to radomly color all the vertices of the graph and then check for type1 and type2 errors.
##### Type 1 error occurs when two adjecent vertices are assigned the same color.
##### Type 2 error occurs when more than β neighbors of a vertex are assigned the same color.
&nbsp;

        done ← 0
        for eachv in V do
            randomly color v
        end for
        while done != 1 do
            done ←  checkforType1error();
            if done == 0 then
                continue
            end if
            done ←  checkforType2Error();
        end while
