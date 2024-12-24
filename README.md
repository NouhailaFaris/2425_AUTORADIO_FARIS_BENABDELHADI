# 2425_AUTORADIO_FARIS_BENABDELHADI-
Réalisé par FARIS et BENABDELHADI

# Objectif TP

Ce TP vise à développer un système d'autoradio en utilisant une carte STM32 NUCLEO_L476RG avec plusieurs périphériques. L'objectif est de maîtriser la configuration matérielle et logicielle, ainsi que l'intégration des protocoles de communication et des fonctionnalités audio.

# 1 Démarrage 

1-Réference GPIO Expander: MCP23S17-E/SO

2-SPI 1 est utilisé sur le STM32 

3/4-Activation de la fonction printf pour le débogage.

![WhatsApp Image 2024-11-29 at 16 49 13](https://github.com/user-attachments/assets/1ea06745-c192-49a0-85aa-93f2810e6a95)

5- Activation et configuration de FreeRTOS en mode CMSIS V1.

![image](https://github.com/user-attachments/assets/54373661-c2ab-4fd3-921c-7e0393af262d)

6- Mise en place d'un shell fonctionnel :

![WhatsApp Image 2024-11-29 at 16 49 13](https://github.com/user-attachments/assets/91c9f921-2b21-42b3-a5ed-78e3a3b188ea)

![WhatsApp Image 2024-11-29 at 16 56 05](https://github.com/user-attachments/assets/f0fa2efb-67ac-48bc-bc9c-b6b3a79fa6ae)

# 2 Le GPIO Expander et le VU-Metre

## 2.1 Configuration

1-Référence du GPIO Expander : MCP23S17

![image](https://github.com/user-attachments/assets/38418675-5ab3-4d69-b5c0-69a321d16b02)

2- Le SPI 1 est deja utilisé par le STM32

3- Paramètres STM32CubeIDE :

. Configuration du SPI en mode maître.

. Horloge configurée à 1 MHz.

. Données envoyées en MSB first.

. Mode CPOL = 0 et CPHA = 1 Edge.

![image](https://github.com/user-attachments/assets/723572ae-4657-4ad3-933d-c4e695a57bdf)

![image](https://github.com/user-attachments/assets/b3cfbfe9-081d-4cc1-a1e4-334830acffca)

4-configuration sur STM32

![image](https://github.com/user-attachments/assets/3271d4c8-f29a-4b7b-a30a-5862cbd4b577)

![image](https://github.com/user-attachments/assets/be5ced25-f650-4c0f-9ea2-898547a8e047)


## 2.2 Tests

**Remarque**

Pour Tester on a fixer notre NRESET à 1 par un GPIO out (PA0).

![WhatsApp Image 2024-12-03 at 10 49 06](https://github.com/user-attachments/assets/a01e39d6-ecfb-473b-add9-7959a40b2433)

![WhatsApp Image 2024-12-03 at 11 24 23](https://github.com/user-attachments/assets/76d48152-f890-4c09-ad66-b5fd9d58ca21)

## un chenillard 


![image](https://github.com/user-attachments/assets/e74f3bcb-2989-41df-b8e6-621c9ef47de4)

![image](https://github.com/user-attachments/assets/4f3ae65a-d6af-47c6-97bd-3d273e28a9d5)

## 2.3  Driver

### Utilisation du Driver avec le shell 

Écriture d'un driver permettant de piloter les LED avec une structure.

Fonctionnalités du shell :

Commande 'a': Allume toutes les LED.

Commande 'l': Accepte deux arguments :

Le premier argument pour identifier la LED spécifique.

Le second argument pour l'état (1 pour allumer, 0 pour éteindre).

![image](https://github.com/user-attachments/assets/0e95ceca-a1b2-4067-bc5b-c525b3b96929)


# 3. Le CODEC Audio SGTL5000
### 3. 1 Configuration préalables

Pins utilisées pour l’I2C:

. I2C_SCL : PB10

. I2C_SDA : PB11

Ces broches correspondent au I2C2 dans le STM32 L476RG.

### Configuration de l'I2C pour la gestion des registres du CODEC.

![image](https://github.com/user-attachments/assets/088c63a5-67f1-4265-9644-69022190d343)

![image](https://github.com/user-attachments/assets/1ffd857c-37e3-4d9d-abd4-fd437adf69e1)

### Vérification de l'horloge MCLK

![scope_1](https://github.com/user-attachments/assets/15dea245-b1a5-43cf-a54c-e2c7bacb927a)

### Tests :

## Lecture du registre CHIP_ID via I2C et vérification des trames sur oscilloscope :

![image](https://github.com/user-attachments/assets/5ec7929f-fee0-4a5a-a459-cbe490cafe41)

![image](https://github.com/user-attachments/assets/7c6f0cca-d996-4794-b3c7-0dd7e8ae576d)

### Trames I2C 

![scope_0](https://github.com/user-attachments/assets/97d2ad78-aa4f-4438-bae0-850677dc9583)

###  la valeur à assigner aux registres : 

— CHIP_ANA_POWER: 0x6AFF

— CHIP_LINREG_CTRL: 0x006C

— CHIP_REF_CTRL: 0x004E

— CHIP_LINE_OUT_CTRL: 0x0322
  
— CHIP_SHORT_CTRL: 0x1106

— CHIP_ANA_CTRL: 0x0133

— CHIP_DIG_POWER: 0x0073

— CHIP_LINE_OUT_VOL: 0x0505

— CHIP_CLK_CTRL: 0x0002

— CHIP_I2S_CTRL: 0x0001

— CHIP_ADCDAC_CTRL:0x000C

— CHIP_DAC_VOL: 0x3C3C

### Fonction utiliser dans fichier sgtl5000

**Fonction d'initialisation du SGTL5000**
```
HAL_StatusTypeDef sgtl5000_init(h_sgtl5000_t *h_sgtl5000) {
    HAL_StatusTypeDef ret = HAL_OK;
    uint16_t mask;

    // Désactivation de l'alimentation initiale
    mask = (1 << 12) | (1 << 13);
    ret = sgtl5000_i2c_clear_bit(h_sgtl5000, SGTL5000_CHIP_ANA_POWER, mask);
    if (ret != HAL_OK) return ret;

    // Activation du charge pump
    mask = (1 << 5) | (1 << 6);
    ret = sgtl5000_i2c_set_bit(h_sgtl5000, SGTL5000_CHIP_LINREG_CTRL, mask);
    if (ret != HAL_OK) return ret;

    // Configuration des tensions de référence
    mask = 0x01FF; // VAG_VAL = 1.575V, BIAS_CTRL = -50%, SMALL_POP = 1
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_REF_CTRL, mask);
    if (ret != HAL_OK) return ret;

    // Configuration de la sortie ligne
    mask = 0x031E;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_LINE_OUT_CTRL, mask);
    if (ret != HAL_OK) return ret;

    // Configuration des courts-circuits
    mask = 0x1106;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_SHORT_CTRL, mask);
    if (ret != HAL_OK) return ret;

    // Activation des blocs
    mask = 0x6AFF;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_ANA_POWER, mask);
    if (ret != HAL_OK) return ret;

    mask = 0x0073;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_DIG_POWER, mask);
    if (ret != HAL_OK) return ret;

    // Configuration des volumes
    mask = 0x1111;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_LINE_OUT_VOL, mask);
    if (ret != HAL_OK) return ret;

    // Configuration de l'horloge
    mask = 0x0004;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_CLK_CTRL, mask);
    if (ret != HAL_OK) return ret;

    // Configuration des données I2S
    mask = 0x0130;
    ret = sgtl5000_i2c_write_register(h_sgtl5000, SGTL5000_CHIP_I2S_CTRL, mask);
    if (ret != HAL_OK) return ret;

    return ret;
}

```
**Fonction pour écrire dans un registre du SGTL5000**
```
HAL_StatusTypeDef sgtl5000_i2c_write_register(h_sgtl5000_t *h_sgtl5000, uint16_t reg, uint16_t value) {
    uint8_t data[4];
    data[0] = (reg >> 8) & 0xFF;
    data[1] = reg & 0xFF;
    data[2] = (value >> 8) & 0xFF;
    data[3] = value & 0xFF;

    return HAL_I2C_Master_Transmit(h_sgtl5000->hi2c, h_sgtl5000->i2c_address, data, 4, HAL_MAX_DELAY);
}
```
**Fonction pour lire un registre du SGTL5000**
```
HAL_StatusTypeDef sgtl5000_i2c_read_register(h_sgtl5000_t *h_sgtl5000, uint16_t reg, uint16_t *value) {
    uint8_t reg_data[2] = { (reg >> 8) & 0xFF, reg & 0xFF };
    uint8_t data[2];

    HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(h_sgtl5000->hi2c, h_sgtl5000->i2c_address, reg_data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;

    ret = HAL_I2C_Master_Receive(h_sgtl5000->hi2c, h_sgtl5000->i2c_address, data, 2, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;

    *value = (data[0] << 8) | data[1];
    return HAL_OK;
}
```
#### Test de Notre configuration
![image](https://github.com/user-attachments/assets/38343d67-56f7-4a84-867b-46fa23631baa)

### Signaux I2S:

**Démarrage la réception et la transmission sur l’I2S avec le SAI :**

![image](https://github.com/user-attachments/assets/41c3bbe2-54c8-44bb-a17b-c9f75f2f23f0)

### Observez à l’oscilloscope les différents signaux d’horloge:

## Génération de signal audio
### Configuration du  DAC (Digital-to-Analog Converter):

Notre microcontrôleur STM32 possède un convertisseur numérique-analogique (DAC) qui vas être utilisé pour générer des formes d'onde analogiques (un signal triangulaire) .

![image](https://github.com/user-attachments/assets/77c5d397-bbdf-416a-b00e-ef1cdd255ab0)

### Bypass numérique:

***lecture les échantillons de l’ADC, et de l'écriture sur le DAC***

![image](https://github.com/user-attachments/assets/e024d653-a6f7-407f-acde-8d398acdc963)

## 5. Filtre RC
**Expressions de A, B et D :**
𝑅𝐶=1/2𝜋𝑓𝑐
A=fs (fréquence d'échantillonnage)
𝐵=𝑅𝐶
𝐷=𝑅𝐶+1/𝑓𝑠

**Cycles processeur disponibles :**
Avec une fréquence d'échantillonnage de 48 kHz et un processeur à 170 MHz :
Temps par échantillon :
𝑇=1/48000 ≈ 20.83𝜇𝑠
Cycles processeur :
170×10^6x20.83x10^(−6) ≈ 3541𝑐𝑦𝑐𝑙𝑒𝑠

**Fonction d’initialisation :**
RC_filter_init() :
```
void RC_filter_init(h_RC_filter_t *h_RC_filter, uint16_t cutoff_frequency, uint16_t sampling_frequency) {
    float RC = 1.0 / (2 * 3.1416 * cutoff_frequency);
    float dt = 1.0 / sampling_frequency;

    h_RC_filter->coeff_A = (uint32_t)(dt / (RC + dt) * 65536);
    h_RC_filter->coeff_B = (uint32_t)(RC / (RC + dt) * 65536);
    h_RC_filter->coeff_D = 65536;
    h_RC_filter->out_prev = 0;
}
```
Commande Shell :
Pour modifier la fréquence de coupure depuis le Shell :
```
void shell_set_cutoff(h_RC_filter_t *h_RC_filter, uint16_t cutoff, uint16_t sampling) {
    RC_filter_init(h_RC_filter, cutoff, sampling);
}

```

