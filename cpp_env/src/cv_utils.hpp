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

void draw_rotatedRect(cv::InputOutputArray image, const cv::RotatedRect& rt)
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
    ButtonArray(double fontScale = 0.8, int thickness = 2, int fontFace = cv::FONT_HERSHEY_SIMPLEX) : fontScale(fontScale),
                                                                                            thickness(thickness),
                                                                                            fontFace(fontFace) {}

    template<typename ...T>
    void add_exclusiveButtons(T ... args) {
        add_buttons(args...);
        std::vector<cv::String> buttonNames;
        buttonNames.reserve(sizeof...(args));
        (void) std::initializer_list<int>{(buttonNames.push_back(args), 0)...};
        exclusive_buttons.push_back(std::move(buttonNames));
    }



    template <typename ... T>
    void add_buttons(T ... args){
        static_assert(std::conjunction_v<std::is_constructible<cv::String, T>...>);
        (void) std::initializer_list<int>{(addButton(args), 0)...};
    }

    void flip(const cv::String &buttonName) {
        if (!exclusive_buttons.empty()) {
            size_t index_i = 0, index_j = 0;
            bool foundExclusive = false;
            for (size_t i = 0; i != exclusive_buttons.size(); i++) {
                for (size_t j = 0; j != exclusive_buttons[i].size(); j++) {
                    if (buttonName == exclusive_buttons[i][j]) {
                        index_i = i;
                        index_j = j;
                        foundExclusive = true;
                        break;
                    }
                }
                if (foundExclusive) break;
            }
            if (!foundExclusive) {
                buttons[buttonName].second = !buttons[buttonName].second;
            }else{
                // Only flip this exclusive button if it was OFF, else do nothing
                if (!buttons[buttonName].second) {
                    for (size_t j = 0; j != exclusive_buttons[index_i].size(); j++) {
                        if (j != index_j) buttons[exclusive_buttons[index_i][j]].second = false;
                    }
                    buttons[buttonName].second = true;
                }
            }
        }else{
            buttons[buttonName].second = !buttons[buttonName].second;
        }

    }

    void imshow(const cv::String &winName, cv::InputOutputArray canvas) {
        auto mouse_callBack = [](int event, int x, int y, int flags, void *userdata) {
            auto &self = *static_cast<ButtonArray *>(userdata);
            if (event == cv::EVENT_LBUTTONDOWN) {
                for (const auto &[buttonName, button] : self.getButtons()) {
                    const auto &bx = button.first.x;
                    const auto &by = button.first.y;
                    const auto &bw = button.first.width;
                    const auto &bh = button.first.height;
                    if (x > bx and x < bx + bw and y > by and y < by + bh) {
                        self.flip(buttonName);
                    }
                }
            }
        };

        for (const auto &button : buttons) {
            if (button.second.second) {
                rectangle(canvas, button.second.first, {0,200,0}, -1);
            }
            rectangle(canvas, button.second.first, {0,0,255}, 1);
            putText(canvas, button.first, {button.second.first.x, button.second.first.y + height - baseline}, fontFace,
                    fontScale, 0, thickness);
        }
        cv::namedWindow(winName);
        cv::setMouseCallback(winName, mouse_callBack, static_cast<void *>(this));
        cv::imshow(winName, canvas);
    }

    bool &getState(const cv::String &key) {
        return buttons[key].second;
    }

    const std::map<cv::String, std::pair<cv::Rect2i, bool>> &getButtons() const {
        return buttons;
    }

private:

    std::map<cv::String, std::pair<cv::Rect2i, bool>> buttons;
    std::vector<std::vector<cv::String>> exclusive_buttons;
    double fontScale;
    int thickness, fontFace, baseline = 0, height = 0;
    int maxWidth = -1;
    int top = 5, left = 5;

    void addButton(const cv::String &buttonName) {
        assert(!is_duplicated(buttonName));
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

    bool is_duplicated(const cv::String& name){
        bool duplicated = false;
        for (const auto &p : buttons) {
            if (p.first == name) {
                duplicated = true;
                break;
            }
        }
        return duplicated;
    }
};

void label_boundingBox(cv::InputOutputArray canvas, const cv::Rect2i &box, const cv::String &text, const cv::Scalar& backgroundColor = {255,0,0}, const cv::Scalar& boxColor = {0,255,0}){
    constexpr double fontScale = 0.6;
    constexpr int thickness = 1;
    constexpr int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    int baseline = 0;
    cv::Size2i sz = cv::getTextSize(text, fontFace, fontScale, thickness, &baseline);
    const int height = baseline + sz.height + thickness;
    cv::rectangle(canvas, {box.x, box.y - height, sz.width, height}, backgroundColor, -1);
    cv::rectangle(canvas, box, boxColor, 1);
    cv::putText(canvas, text, {box.x, box.y - baseline}, fontFace, fontScale, {255, 255, 255}, thickness);
}
