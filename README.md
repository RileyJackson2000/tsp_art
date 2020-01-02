<script type="text/javascript" async
src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.2/MathJax.js? 
config=TeX-MML-AM_CHTML">
</script>
# tsp_art
Solves the travelling salesman problem (in an artistic way!)

Takes as input a png file and uses a modified Poisson-disk sampling method to stipple image, then treats the result as an instance of the metric tsp on a euclidean graph and finds an approximate solution using Christofides algorithm.

## Usage
(cmake is a work in progress so for now you need to manually compile and link, the output of the below comes from pkg-config so I'm not really sure what is needed)

~~~
$ g++ -std=c++14 -Wall -g -O3 renderer.cpp image.cpp poisson.cpp tsp.cpp -lcairo -I /usr/include/cairo -L/usr/local/lib -lopencv_gapi -lopencv_stitching -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_highgui -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_line_descriptor -lopencv_quality -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core -o tsp_art
~~~

To invoke, use either of
~~~
$ ./tsp_art input.png lines_out.png
$ ./tsp_art input.png lines_out.png dots_out.png
~~~

## Part 1: Poisson Disk Sampling (PDS)

For visualization purposes, the problem to be solved here is to transforming the picture on the left into something like the picture on the right.
<p align="center">
  <img src="./pics/ghost.png" width="400" /> 
  <img src="./pics/ghost_dots.png" width="400" />
</p>

Treating each pixel as an indicator variable with probability proportional to its grayscale density would yield a truly random stippling of said image. However, such a method produces random clustering which is visually unappealing. Instead, computer graphics applications typically use a blue noise distribution, ie a white noise distribution with an enforced minimum distance between points; this produces a much better looking, but still random, stippling. Achieving said blue noise distribution is done many ways in practice but I chose to use PDS because it was the most intuitive to me, and since other methods such as weighted voronoi stippling and ordered dithering have already been used in similar projects.

### So what is Poisson Disk Sampling?

Mathematically, given an image $$I=W\times H$$

## Part 2: Christofides
## Part 3: Rendering
