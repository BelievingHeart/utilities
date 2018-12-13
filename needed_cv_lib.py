
cv_modules=["opencv_calib3d",
        "opencv_core",
        "opencv_dnn",
        "opencv_features2d",
        "opencv_flann",
        "opencv_gapi",
        "opencv_highgui",
        "opencv_imgcodecs",
        "opencv_imgproc",
        "opencv_ml",
        "opencv_objdetect",
        "opencv_photo",
        "opencv_stitching",
        "opencv_video",
        "opencv_videoio",
        "opencv_aruco",
        "opencv_bgsegm",
        "opencv_bioinspired",
        "opencv_ccalib",
        "opencv_datasets",
        "opencv_dnn_objdetect",
        "opencv_dpm",
        "opencv_face",
        "opencv_freetype",
        "opencv_fuzzy",
        "opencv_hfs",
        "opencv_img_hash",
        "opencv_line_descriptor",
        "opencv_optflow",
        "opencv_phase_unwrapping",
        "opencv_plot",
        "opencv_reg",
        "opencv_rgbd",
        "opencv_saliency",
        "opencv_shape",
        "opencv_stereo",
        "opencv_structured_light",
        "opencv_superres",
        "opencv_surface_matching",
        "opencv_text",
        "opencv_tracking",
        "opencv_videostab",
        "opencv_xfeatures2d",
        "opencv_ximgproc",
        "opencv_xobjdetect",
        "opencv_xphoto"]

redundent_modules=[]
with open("./LWYU_out.txt", "r") as f:
     f_contests = f.readlines()
     for m in cv_modules:
         for line in f_contests:
             if m in line:
                 redundent_modules.append(m)
                 break

needed_modules = list(set(cv_modules) - set(redundent_modules))
needed_modules = " ".join(needed_modules)
print(needed_modules)


