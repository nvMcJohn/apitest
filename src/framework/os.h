#pragma once

namespace os {
    void chdir(std::string _newdir);

    namespace path {
        std::string dirname(std::string _path);
    };
};