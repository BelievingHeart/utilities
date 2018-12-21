
cv_modules=["aruco", "bgsegm", "bioinspired", "calib3d", "ccalib", "core", "cudaarithm", "cudabgsegm", "cudacodec", "cudafeatures2d", "cudafilters", "cudaimgproc", "cudalegacy", "cudaobjdetect", "cudaoptflow", "cudastereo", "cudawarping", "cudev", "cvv", "datasets", "dnn", "dnn_objdetect", "dpm", "face", "features2d", "flann", "freetype", "fuzzy", "gapi", "hfs", "highgui", "img_hash", "imgcodecs", "imgproc", "java_bindings_generator", "line_descriptor", "ml", "objdetect", "optflow", "phase_unwrapping", "photo", "plot", "python_bindings_generator", "reg", "rgbd", "saliency", "shape", "stereo", "stitching", "structured_light", "superres", "surface_matching", "text", "tracking", "ts", "video", "videoio", "videostab", "xfeatures2d", "ximgproc", "xobjdetect", "xphoto"]

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


