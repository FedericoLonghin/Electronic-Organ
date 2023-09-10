

void Wavetable_reloadTable() {

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SIN][i] = sin((i / (float)Wavetable_Length) * 2 * PI) * Wavetable_MaxAmplitude_val / 4.0f + Wavetable_MaxAmplitude_val / 2;
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    if (i <= Wavetable_Length / 2) {
      Wavetable_table[WAVETYPE_TRIANG][i] = ((i / (float)(Wavetable_Length / 2)) * (Wavetable_MaxAmplitude_val / 2)) + Wavetable_MaxAmplitude_val / 4;

    } else {
      Wavetable_table[WAVETYPE_TRIANG][i] = Wavetable_table[WAVETYPE_TRIANG][Wavetable_Length - i];
    }
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SAW][i] = (i / (float)Wavetable_Length) * Wavetable_MaxAmplitude_val / 4.0f;
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SQUARE][i] = i < Wavetable_Length / 2 ? Wavetable_MaxAmplitude_val * 3 / 4.0f : Wavetable_MaxAmplitude_val * 1 / 4.0f;
  }

  for (int i = 0; i < Wavetable_Length; i++) {
    Wavetable_table[WAVETYPE_SIN_3][i] = (sin((i / (float)Wavetable_Length) * 2 * PI) + sin((i / (float)Wavetable_Length) * 4 * PI) + sin((i / (float)Wavetable_Length) * 8 * PI)) * Wavetable_MaxAmplitude_val / 12.0f + Wavetable_MaxAmplitude_val / 2;
  }
}
