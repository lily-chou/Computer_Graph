# Computer_Graph

## Lab 1
Use the keyboard to switch the status of the program. <br>
Use the mouse to click the coordinates assigned to the graph. 
- d: Draw dot
- l: Draw line
- p: Draw polygon
- o: Draw circle
- r: Restore
- q: Quit the progarm

## Lab 2
Use `2021CG_Lab2.exe inputfile.in` to run the program.
1. scale: x y
2. rotate: degree
3. translate: x y
4. square
5. triangle
6. view: wxl wxr wyb wyt vxl vxr vyb vyt
7. clearData
8. clearScreen
9. reset
10. end
11. #command
  > #create a square and its transformation
  > <br>reset
  > <br>scale 2.0 2.0
  > <br>rotate 45.0
  > <br>translate 10.0 10.0
  > <br>square
  > <br>
  > <br>#create a triangle and its transformation
  > <br>reset
  > <br>translate 5.0 5.0
  > <br>scale 0.5 0.5
  > <br>rotate 20.0
  > <br>triangle
  > <br>
  > <br>#2 non-overlapping viewports
  > <br>view 0.0 20.0 0.0 20.0 10.0 40.0  10.0 40.0
  > <br>view 0.0 20.0 0.0 20.0 50.0 100.0 10.0 60.0
  > <br>
  > <br>#clear the openGL window
  > <br>clearScreen
  > <br>
  > <br>#display the world in one viewport
  > <br>view 0.0 10.0 0.0 10.0 10.0 40.0  10.0 40.0
  > <br>
  > <br>#clear data and screen
  > <br>clearData
  > <br>clearScreen
  > <br>
  > <br>#create a new square and it's transformation
  > <br>reset
  > <br>scale 5.0 5.0
  > <br>translate 5.0 5.0
  > <br>square
  > <br>
  > <br>#view the new square in a non-square viewport
  > <br>view 0.0 10.0 0.0 10.0 0.0 100.0 0.0 50.0 
  > <br>
  > <br>end

## Lab3
Use `2021CG_Lab3.exe inputfile.in` to run the program.
1. scale: x y z
2. rotate: x y z
3. translate: x y z
4. object: object.asc
5. observer: Ex Ey Ez COIx COIy COIz Tilt Hither Yon Hav
6. viewport: vxl vxr vyb vyt (即Vxmin Vxmax Vymin Vymax)
7. display
8. nobackfaces
9. Reset: Initialization of Transformation Matrix
10. end
11. #command(Note)

## Lab4
Use `2021CG_Lab4.exe inputfile.in` to run the program.
1. scale: x y z
2. rotate: x y z
3. translate: x y z
4. observer: Ex Ey Ez COIx COIy COIz Tilt Hither Yon Hav
5. viewport: vxl vxr vyb vyt (即Vxmin Vxmax Vymin Vymax)
6. display
7. reset : Initialize the Transformation Matrix 
8. end
9. #command
10. object: object.asc Or Og Ob Kd Ks N
11. ambient: KaIar KaIag KaIab
12. background: Br Bg Bb
13. light: index Ipr Ipg Ipb Ix Iy Iz
