namespace db
{
void imshow(const cv::String &winName_, const cv::Mat &img_)
{
#ifdef DEBUG
    if (img_.depth() == CV_8U)
    {
        cv::imshow(winName_, img_);
    }
    else
    {
        double min_ = 0, max_ = 0;
        cv::Point2i min_pt, max_pt;
        cv::Mat temp;
        cv::minMaxLoc(img_, &min_, &max_, &min_pt, &max_pt, cv::noArray());
        cv::normalize(img_, temp, 0, 255, cv::NORM_MINMAX, CV_8U, {});

        cv::imshow(winName_, temp);
        if (min_ < 0 || max_ > 1)
        {
            fmt::print("[DEBUG] {} is not of any showing formats, a makeshift image "
                       "is create.\nOriginal states:\n",
                       winName_);
            fmt::print("minVal: {} at point ({},{})\n", min_, min_pt.x, min_pt.y);
            fmt::print("maxVal: {} at point ({},{})\n\n", max_, max_pt.x, max_pt.y);
        }
    }
#endif // DEBUG
}

void draw_rotatedRect(InputOutputArray image, const RotatedRect& rt)
{
    cv::Scalar color = cv::Scalar(255.0, 0.0, 255.0); // white
    cv::Point2f vertices2f[4];
    rt.points(vertices2f);
    cv::Point vertices[4];
    for(int i = 0; i < 4; ++i){
        vertices[i] = vertices2f[i];
    }
    cv::fillConvexPoly(image, vertices, 4, color);
}
}; // namespace db

struct ButtonArray {
    ButtonArray(double fontScale, int thickness, int fontFace = cv::FONT_HERSHEY_SIMPLEX) : fontScale(fontScale), thickness(thickness), fontFace(fontFace) {}
    void addButton(const cv::String& buttonName) {
        const cv::Size sz = cv::getTextSize(buttonName, fontFace, fontScale, thickness, &baseline);
        // initialize height for the first push_back
        if (buttons.empty()) {
            height = baseline + sz.height + thickness;
        }
        // if new box is the widest, updates all boxes' width
        if (sz.width > maxWidth) {
            maxWidth = sz.width;
            buttons[buttonName] = {cv::Rect2i(left, top, maxWidth, height), false};
            for (auto &button : buttons) {
                button.second.first.width = maxWidth;
            }
        } else {
            buttons[buttonName] = {cv::Rect2i(left, top, maxWidth, height), false};
        }
        top += height;
    }

    void imshow(const cv::String& winName, cv::InputOutputArray canvas){
        auto mouse_callBack = [](int event, int x, int y, int flags, void *userdata) {
            if (event == cv::EVENT_LBUTTONDOWN) {
                for (auto &[buttonName ,button] : *static_cast<std::map<cv::String, std::pair<cv::Rect2i, bool>>*>(userdata)) {
                    const auto& bx = button.first.x;
                    const auto& by = button.first.y;
                    const auto& bw = button.first.width;
                    const auto& bh = button.first.height;
                    if (x > bx and x < bx + bw and y > by and y < by + bh) {
                        button.second = !button.second;
                        fmt::print("Set <{}> to {}\n", buttonName, button.second ? "ON" : "OFF");
                    }
                }
            }
        };

        for (const auto& button : buttons) {
            rectangle(canvas, button.second.first, {255, 0, 0}, 2);
            putText(canvas, button.first, {button.second.first.x, button.second.first.y + height - baseline}, fontFace, fontScale, 0, thickness);
        }
        cv::namedWindow(winName);
        cv::setMouseCallback(winName, mouse_callBack, static_cast<void *>(&buttons));
        cv::imshow(winName, canvas);
    }

    bool& getState(const cv::String& key){
        return buttons[key].second;
    }

  private:
    std::map<cv::String, std::pair<cv::Rect2i, bool>> buttons;
    double fontScale;
    int thickness, fontFace, baseline = 0, height = 0;
    int maxWidth = -1;
    int top = 5, left = 5;

};