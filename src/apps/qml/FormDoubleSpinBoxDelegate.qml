// SPDX-FileCopyrightText: 2026 Onuralp SEZER <thunderbirdtr@fedoraproject.org>
//
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

import QtQuick
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormSpinBoxDelegate {
    id: root

    property int decimals: 2
    property real realValue: 0
    property real realFrom: 0
    property real realTo: 1
    property real realStepSize: 0.1

    signal realValueModified(real value)

    readonly property real factor: Math.pow(10, decimals)
    readonly property int scaledValue: Math.round(realValue * factor)

    property bool completed: false

    from: Math.round(realFrom * factor)
    to: Math.round(realTo * factor)
    stepSize: Math.round(realStepSize * factor)

    onScaledValueChanged: if (completed) {
        value = scaledValue;
    }

    onValueChanged: if (completed && value !== scaledValue) {
        realValueModified(value / factor);
    }

    Component.onCompleted: {
        value = scaledValue;
        completed = true;
    }

    textFromValue: (value, locale) => Number(value / factor).toLocaleString(locale, 'f', decimals)
    valueFromText: (text, locale) => Math.round(Number.fromLocaleString(locale, text) * factor)

    validator: DoubleValidator {
        bottom: root.realFrom
        top: root.realTo
        decimals: root.decimals
        notation: DoubleValidator.StandardNotation
    }
}
