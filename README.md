# GraphicsProcessor-HPS-Kernel-Library
Biblioteca para o processador gráfico projetado, destinada a ser usada com o HPS (Hard Processor System) na plataforma DE1-SoC.
Para calcular as potências ativa, reativa, aparente e o fator de potência do circuito para as frequências de 10 Hz, 5 kHz e 5 MHz, utilizaremos as seguintes fórmulas:

1. **Potência Aparente (S):**
\[ S = V_{eficaz} \times I_{eficaz} \]
2. **Potência Ativa (P):**
\[ P = V_{eficaz} \times I_{eficaz} \times \cos(\theta) \]
3. **Potência Reativa (Q):**
\[ Q = V_{eficaz} \times I_{eficaz} \times \sin(\theta) \]
4. **Fator de Potência (FP):**
\[ FP = \cos(\theta) \]

Vamos calcular para cada frequência mencionada:

### Frequência de 10 Hz

- \( V_{eficaz} = 0 \) V
- \( I_{eficaz} = 0 \) mA
- \( \theta = - \)

**Potência Aparente (S):**
\[ S = 0 \times 0 = 0 \, VA \]

**Potência Ativa (P):**
\[ P = 0 \times 0 \times \cos(0) = 0 \, W \]

**Potência Reativa (Q):**
\[ Q = 0 \times 0 \times \sin(0) = 0 \, VAR \]

**Fator de Potência (FP):**
\[ FP = \cos(0) = 1 \]

### Frequência de 5 kHz

- \( V_{eficaz} = 84,85 \) mV = 0,08485 V
- \( I_{eficaz} = 26,58 \) mA = 0,02658 A
- \( \theta = 0^\circ \) (não há defasagem dada)

**Potência Aparente (S):**
\[ S = 0,08485 \times 0,02658 = 0,002255 \, VA \]

**Potência Ativa (P):**
\[ P = 0,08485 \times 0,02658 \times \cos(0) = 0,002255 \, W \]

**Potência Reativa (Q):**
\[ Q = 0,08485 \times 0,02658 \times \sin(0) = 0 \, VAR \]

**Fator de Potência (FP):**
\[ FP = \cos(0) = 1 \]

### Frequência de 5 MHz (5.000 kHz)

- \( V_{eficaz} = 7,877 \) mV = 0,007877 V
- \( I_{eficaz} = 178,19 \) mV = 0,17819 A
- \( \theta = 0,04^\circ \) (ou 8 ns)

**Potência Aparente (S):**
\[ S = 0,007877 \times 0,17819 = 0,001403 \, VA \]

**Potência Ativa (P):**
\[ P = 0,007877 \times 0,17819 \times \cos(0,04) \]
\[ P \approx 0,007877 \times 0,17819 \times 0,99999975 = 0,001403 \, W \]

**Potência Reativa (Q):**
\[ Q = 0,007877 \times 0,17819 \times \sin(0,04) \]
\[ Q \approx 0,007877 \times 0,17819 \times 0,0006981 = 0,0000009 \, VAR \]

**Fator de Potência (FP):**
\[ FP = \cos(0,04) = 0,99999975 \]

Resumindo:

- **10 Hz:**
  - \( S = 0 \, VA \)
  - \( P = 0 \, W \)
  - \( Q = 0 \, VAR \)
  - \( FP = 1 \)

- **5 kHz:**
  - \( S = 0,002255 \, VA \)
  - \( P = 0,002255 \, W \)
  - \( Q = 0 \, VAR \)
  - \( FP = 1 \)

- **5 MHz:**
  - \( S = 0,001403 \, VA \)
  - \( P = 0,001403 \, W \)
  - \( Q = 0,0000009 \, VAR \)
  - \( FP = 0,99999975 \)
