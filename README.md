# 2425_AUTORADIO_FARIS_BENABDELHADI-
Réalisé par FARIS et BENABDELHADI
# 1 Démarrage 

1-Réference GPIO Expander: MCP23S17-E/SO

2-SPI 1 est utilisé sur le STM32 

3/4-
![WhatsApp Image 2024-11-29 at 16 49 13](https://github.com/user-attachments/assets/1ea06745-c192-49a0-85aa-93f2810e6a95)

5- 

![image](https://github.com/user-attachments/assets/54373661-c2ab-4fd3-921c-7e0393af262d)

6)a-


![WhatsApp Image 2024-11-29 at 16 49 13](https://github.com/user-attachments/assets/91c9f921-2b21-42b3-a5ed-78e3a3b188ea)

6)b-

![WhatsApp Image 2024-11-29 at 16 56 05](https://github.com/user-attachments/assets/f0fa2efb-67ac-48bc-bc9c-b6b3a79fa6ae)

# 2 Le GPIO Expander et le VU-Metre

## 2.1 Configuration

1-

![image](https://github.com/user-attachments/assets/38418675-5ab3-4d69-b5c0-69a321d16b02)

2- Le SPI 1 est deja utilisé par le STM32

3- il faut configurer le SPI MOSI/ SPI MISO/ SPI SCK/ SPI NCS comme dans le modele Kicad

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

## 2.3 Driver 


![image](https://github.com/user-attachments/assets/e74f3bcb-2989-41df-b8e6-621c9ef47de4)

![image](https://github.com/user-attachments/assets/4f3ae65a-d6af-47c6-97bd-3d273e28a9d5)

## Le CODEC Audio SGTL5000
### Configuration préalables
Pins utilisées pour l’I2C:
. I2C_SCL : PB10
. I2C_SDA : PB11
Ces broches correspondent au I2C2 dans le STM32 L476RG.

### Configuration:

![image](https://github.com/user-attachments/assets/088c63a5-67f1-4265-9644-69022190d343)

![image](https://github.com/user-attachments/assets/1ffd857c-37e3-4d9d-abd4-fd437adf69e1)

![image](https://github.com/user-attachments/assets/a522406e-ac0f-4e58-9f31-19dedb4f0a62)
