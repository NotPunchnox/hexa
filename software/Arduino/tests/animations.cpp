float Attack[6][3][3] = {
    // {x, z, y} pour chaque patte {LFL, LML, LBL, LFR, LMR, LBR}
    {{10, 9, 0}, {10, 6, -10}, {10, 9, 0}, {10, 9, 0}, {10, 6, 10}, {10, 9, 0}},//step1
    {{10, 9, 0}, {10, 10, -10}, {10, 7.5, 0}, {10, 9, 0}, {10, 10, 10}, {10, 7.5, 0}},//step2
    {{5, 4.3, 3.6}, {10, 10, -10}, {10, 7.6, 0}, {5.3, 4.9, -1.4}, {10, 10, 10}, {10, 7.6, 0}},//step3
};