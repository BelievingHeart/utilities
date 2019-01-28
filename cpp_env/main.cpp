#include <opencv2/opencv.hpp>
#include <fmt/printf.h>
#include "src/timer.hpp"


int main(const int argc, const char *argv[])
{
    const cv::CommandLineParser parser(
        argc, argv,
        "{help ? h ||}"
        "{@input | /home/afterburner/Downloads/lvlian/lvlian_8.jpeg | input image}"
        "{scale_factor| 0.5| image scale factor}"
        "{rotate | false | true when dealing with things like book pages }");
    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }
    const auto image_path = parser.get<std::string>("@input");
    src = cv::imread(image_path, cv::IMREAD_COLOR);
    if (src.empty())
    {
        fmt::print("Error reading image <{}>\n", image_path);
        return -1;
    }
}
