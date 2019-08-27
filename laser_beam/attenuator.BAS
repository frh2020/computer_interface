PRINT " This program controls the rotation of a circular neutral filter. "
PRINT " Input a value of optical density, the filter will rotate to the right position."
PRINT " Input negative value to quit the program."
PRINT " Have Fun "

PRINT "Please reset filter manually to 0 degree";

mport = 512
mdg = 3.75

dg0pc = 0
od0 = 0
DO
    PRINT "Optical Density:"; od0; "("; pc0; ")",
    INPUT "New Optical Density:"; od
    IF od < 0 THEN EXIT DO
    dg = od
    IF dg - dg0 < 0 THEN pc = pc - 360
    mstep = INT((dg - dg0) / mdg)
    IF mstep < 0 THEN st = -1 ELSE st = 1
    FOR ii = 1 TO mstep STEP st
        cmd$ = "mv3 " + STR$(st) + CHR$(13)
        FOR l = 1 TO LEN(cmd$)
            FOR i = 1 TO 500
            NEXT i
            s = ASC(MID$(cmd$, l, 1))
            OUT mport, s
        NEXT l
        FOR ll = 1 TO 850
        NEXT ll
    NEXT ii
    dg0 = dg0 + mstep * mdg
    od0 = dg0
    LOOP
PRINT
END
