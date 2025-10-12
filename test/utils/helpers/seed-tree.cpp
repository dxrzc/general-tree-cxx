#include "seed-tree.h"
#include <vector>

general_tree<LifecycleCounter> seed_tree(std::size_t max)
{
    if (max == 0)
        return general_tree<LifecycleCounter>();

    general_tree<LifecycleCounter> gt("string0", 0);
    auto root = gt.root();

    // to build level by level
    std::vector<typename general_tree<LifecycleCounter>::node> parents;
    parents.push_back(root);
    std::size_t counter = 1;

    while (counter < max && !parents.empty())
    {
        std::vector<typename general_tree<LifecycleCounter>::node> next_parents;

        for (auto parent : parents)
        {
            if (counter < max)
            {
                // add left child
                const std::string string_value = "string" + std::to_string(counter);
                const int int_value = static_cast<int>(counter);
                
                auto left = gt.emplace_left_child(parent, string_value, int_value);                
                ++counter;

                next_parents.push_back(left);
                
                // add a right sibling
                if (counter < max)
                {
                    const std::string string_value = "string" + std::to_string(counter);
                    const int int_value = static_cast<int>(counter);
                    
                    auto sibling = gt.emplace_right_sibling(left, string_value, int_value);
                    ++counter;

                    next_parents.push_back(sibling);
                }
            }
        }

        parents.swap(next_parents);
    }

    LifecycleCounter::reset();
    return gt;
}