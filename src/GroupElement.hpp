/**
 * @file      GroupElement.hpp
 * @author    Simon Brummer (<simon.brummer@posteo.de>)
 * @brief     Command line argument parsing.
 * @copyright 2017 Simon Brummer. All rights reserved.
 */

/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#ifndef GROUPELEMENT_HPP_201804081223
#define GROUPELEMENT_HPP_201804081223

#include <memory>
#include <cstdint>
#include <optional>
#include "Element.hpp"
#include "ObserverElement.hpp"

// ui element representing a group of hosts
class GroupElement : public Element
{
public:
    using Pointer = std::shared_ptr<GroupElement>;

    // Constructor: A group can have a optional name and a list of
    // ObserverElements associated with this group
    GroupElement( std::optional<std::string> const&            name
                , std::vector<ObserverElement::Pointer> const& observers
                );

    virtual ~GroupElement() = default;

    // Element interface implementation. See Element.hpp
    virtual void draw(Window::Pointer wnd, Position& pos) const override;
    virtual unsigned get_height() const override;
    virtual unsigned get_width() const override;

private:
    std::optional<std::string>            name_;
    std::vector<ObserverElement::Pointer> observers_;
};

#endif // GROUPELEMENT_HPP_201804081223
