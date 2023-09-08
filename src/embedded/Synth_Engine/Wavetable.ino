

void Wavetable_reloadTable() {
  switch (Wavetable_waveForm) {
    case WAVETYPE_SIN:
      for (int i = 0; i < Wavetable_Length; i++) {
        Wavetable_table[WAVETYPE_SIN][i] = sin((i / (float)Wavetable_Length) * 2 * PI) * Wavetable_MaxAmplitude_val / 4.0f + Wavetable_MaxAmplitude_val / 2;
      }
      break;
    case WAVETYPE_TRIANG:
      for (int i = 0; i < Wavetable_Length; i++) {
        if (i <= Wavetable_Length / 2) {
          Wavetable_table[WAVETYPE_TRIANG][i] = ((i / (float)(Wavetable_Length / 2)) * (Wavetable_MaxAmplitude_val / 2)) + Wavetable_MaxAmplitude_val / 4;

        } else {
          Wavetable_table[WAVETYPE_TRIANG][i] = Wavetable_table[WAVETYPE_TRIANG][Wavetable_Length - i];
        }
      }
      break;
    case WAVETYPE_SAW:
      for (int i = 0; i < Wavetable_Length; i++) {
        Wavetable_table[WAVETYPE_SAW][i] = (i / (float)Wavetable_Length) * Wavetable_MaxAmplitude_val / 4.0f;
      }
      break;
    case WAVETYPE_SQUARE:
      for (int i = 0; i < Wavetable_Length; i++) {
        Wavetable_table[WAVETYPE_SQUARE][i] = i < Wavetable_Length / 2 ? Wavetable_MaxAmplitude_val * 3 / 4.0f : Wavetable_MaxAmplitude_val * 1 / 4.0f;
      }
      break;
  }
}
