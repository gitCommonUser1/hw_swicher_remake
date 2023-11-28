/*
 * Copyright (c) 2015-2017 Alex Spataru <alex_spataru@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _QSwitcher_COMMON_H
#define _QSwitcher_COMMON_H

#include <QString>

/**
 * @brief Represents a switcher and its properties
 *
 * This structure contains:
 *     - The numerical ID of the switcher
 *     - The switcher display name
 *     - The number of knobs operated by the switcher
 *     - The number of pushes operated by the switcher
 *     - The number of keys operated by the switcher
 */
struct QSwitcherDevice
{
    int id;             /**< Holds the ID of the switcher */
    QString name;       /**< Holds the name/title of the switcher */

    QList<int> knobs;   /**< Holds the values for each knob */
    QList<int> pushes;  /**< Holds the values for each push */
    QList<int> keys;    /**< Holds the values for each key */
};

/**
 * @brief Represents an Knob event that can be triggered by a switcher
 *
 * This structure contains:
 *    - The knob ID
 *    - The current knob value
 */
struct QSwitcherKnobEvent
{
    int knob;   /**< The ID of the knob */
    int value;  /**< The value (from -N to N without 0) of the push */

    QSwitcherDevice *switcher;
};

/**
 * @brief Represents an Knob event that can be triggered by a switcher
 *
 * This structure contains:
 *    - The Push ID
 *    - The current Push value
 */
struct QSwitcherPushEvent
{
    int push;   /**< The ID of the push */
    int value;  /**< The value (from 0 to 100) of the push */

    QSwitcherDevice *switcher;
};

/**
 * @brief Represents an Key event that can be triggered by a switcher
 *
 * This structure contains:
 *    - The key ID
 *    - The current key value
 */
struct QSwitcherKeyEvent
{
    int key;    /**< The ID of the key */
    int value;  /**< The value (from 0 to 1) of the key */

    QSwitcherDevice *switcher;
};

#endif
