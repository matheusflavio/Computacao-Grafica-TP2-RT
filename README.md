# RayTracing_CG

## TP3 Computer Graphics - UFMG

This project is an implementation of a Distributed Raytracing Algorithm. </br>
For objects, it supports spheres and polyhedrons, besides lights. </br>
For pigments, it supports the checker pattern, solid color and image textures. </br>
The material can be reflective, refractive, diffuse or a mix of all. </br></br>

Input files follow the pattern described in the TP_instructions.pdf file.

### Instructions

- Compile: "make"
- Run: "./demo.o {input_file_path.txt} {output_file_path.ppm} {width} {height} {samples_per_pixel}"
  - width, height and samples_per_pixel are optional
  - A low samples_per_pixel (5 - 10) can be used for fast rendering
  - A high samples_per_pixel (100-500) can be used for high quality time consuming rendering


#### Extra Features input instruction
- Imperfect Reflection and Imperfect Refraction: Add a last parameter to the input file material description, this parameter defines the fuzyness and should range from 0 to 1. It's optional.
- Camera Aperture and Distance to Focus: Add the 2 values at the end of the file (both are floats). 
- Smooth Shadow: auto applied



#### Extra features implemented:

- Smooth Shadow (Example below - Perfect shadow x smooth shadow)

<img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_perfect_shadow.png?raw=true"/> <img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_smooth_shadow.png?raw=true"/>

- No Aperture (input_noaperture.txt) &  Focused (input_focused.txt)

<img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_noaperture.png?raw=true"/> <img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_focused.png?raw=true"/>

- Imperfect Reflection (input_perfect_reflection.txt & input_imperfect_reflection.txt)

<img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_perfect_reflection.png?raw=true"/><img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_imperfect_reflection.png?raw=true"/>

- Imperfect Refraction (input_perfect_refraction.txt & input_imperfect_refraction.txt)

<img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_perfect_refraction.png?raw=true"/><img width="500" src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_imperfect_refraction.png?raw=true"/>

#### Some more examples

input_tp_example.txt
<img src="https://github.com/vini2001/RayTracing_CG/blob/d6040a0565f6ac527273f2f12989efeeb91a63f9/outputs/img_tp_high_res.png?raw=true"/>
input1.txt
<img src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_desctp_plus_checker.png?raw=true"/>
<img src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_light_and_earth.png?raw=true"/>
<img src="https://github.com/vini2001/RayTracing_CG/blob/master/outputs/img_halfrefrrefl.png?raw=true"/>
