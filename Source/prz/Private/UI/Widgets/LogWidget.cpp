// Copyright 2016 Max Molodtsov <zerksud@gmail.com>
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "prz.h"
#include "UI/Widgets/LogWidget.h"


void SLogWidget::Construct(const FArguments& InArgs) {
    OwnerHUD = InArgs._OwnerHUD;

    ChildSlot
    [
        SNew(STextBlock)
        .Font(FSlateFontInfo(FPaths::GameContentDir() / TEXT("Fonts/orbitron-bold.otf"), 24))
        .Text(this, &SLogWidget::GetLogHistory)
    ];
}

void SLogWidget::UpdateLogHistory(const FText& logHistory) {
    mLogHistory = logHistory;
}

FText SLogWidget::GetLogHistory() const {
    return mLogHistory;
}