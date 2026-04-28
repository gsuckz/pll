import re

with open(r'c:\Users\pabli\OneDrive\Desktop\pll\tex\main.tex', 'r', encoding='utf-8') as f:
    content = f.read()

# 1. Update SOP16 to SSOP 16
content = content.replace(
    'mercado con distinto encapsulado, lo que requirió una nueva iteración',
    'mercado con distinto encapsulado (se adquirió por error un chip en formato SSOP 16 en lugar del original SOP), lo que requirió una nueva iteración'
)
content = content.replace(
    'Layout de la PCB rediseñada. Usando la huella  SOP16',
    'Layout de la PCB rediseñada. Usando la huella SSOP 16'
)

# 2. Reemplazo Implementacion hardware prueba
old_hw = r"""	\begin{itemize}\[leftmargin=\*\]
		\item \\textbf\{Fuente de 12\\V y 5\\V:\} Modulos integrados conectados en una PCB aparte\.

		\item \\textbf\{Modulo RF:\} Contiene el SP6769 y el VCO, dispone de conexiones a alimentación y al puerto I2C del ESP32\.
	\\end\{itemize\}"""

new_hw = r"""	\begin{itemize}[leftmargin=*]
		\item \textbf{Alimentación de 12\,V:} Se diseñó y fabricó una fuente regulable de 12\,V utilizando el método de transferencia térmica (planchado) para la creación de la placa de circuito impreso (PCB).
		\item \textbf{Alimentación de 5\,V:} Para la lógica de control, se utilizó directamente la salida regulada de 5\,V provista por la placa de desarrollo ESP32.
		\item \textbf{Interfaz de conexión (Shield ESP32):} Se fabricó una placa adaptadora (tipo \textit{poncho} o \textit{shield}), también mediante el método de transferencia térmica, para interconectar de manera segura la ESP32 con la placa de RF heredada.
	\end{itemize}

	\begin{figure}[H]
		\centering
		\begin{subfigure}{0.45\textwidth}
			\centering
			\includegraphics[width=\linewidth]{images/image32.png}
			\caption{Vista superior}
		\end{subfigure}
		\hfill
		\begin{subfigure}{0.45\textwidth}
			\centering
			\includegraphics[width=\linewidth]{images/image34.png}
			\caption{Vista inferior}
		\end{subfigure}
		\caption{Shield ESP32 implementado provisionalmente.}
		\label{fig:shield_esp32}
	\end{figure}"""

content = re.sub(old_hw, new_hw, content)


# 3. Remover TODO del montaje
content = content.replace(
    r'\TODO{Agregar fotos del proceso de montaje y del gabinete impreso en 3D.}',
    ''
)

# 4. Remover TODO de resultados barrido
content = content.replace(
    r'de forma continua y estable. \TODO{Agregar resultados cuantitativos del barrido si se dispone de ellos.}',
    r'de forma continua y estable.'
)

# 5. Insertar hardware principal y banco (reemplazar placeholder)
old_banco = r"""	\\begin\{figure\}\[H\]
		\\centering
		\\fcolorbox\{azulUNT\}\{grisClaro\}\{%
			\\parbox\{0\.85\\textwidth\}\{%
				\\centering\\vspace\{2\.5cm\}
				\\textcolor\{azulUNT\}\{\\textbf\{\[PLACEHOLDER: Foto del banco de microondas durante las mediciones\]\}\}\\\\
				\\small Configuración del ensayo: ondámetro, guía de onda, sistema sintetizador\.\\\[2\.5cm\]
			\}
		\}
		\\caption\{Banco de microondas utilizado para la validación funcional\.\}
		\\label\{fig:banco_microondas\}
	\\end\{figure\}"""

new_banco = r"""	\subsection{Hardware principal y transmisión de la señal}
	
	El sistema se centra en la generación, transmisión y recepción de ondas electromagnéticas en el rango de las microondas (banda Ku). El núcleo de emisión es el sintetizador operando en un rango de 10{,}6\,GHz a 11{,}8\,GHz. Para acondicionar y caracterizar la señal, el banco cuenta con atenuadores de baja precisión (hasta 20\,dB) y de alta precisión (ajuste micrométrico de 0{,}01\,mm), además de un ondámetro de cavidad resonante.
	
	\begin{figure}[H]
		\centering
		\includegraphics[width=0.7\textwidth]{images/image25.png}
		\caption{Configuración del banco de microondas.}
		\label{fig:banco_original}
	\end{figure}

    La onda viaja por la guía hasta la antena transmisora, la cual está conformada por un dipolo y un reflector parabólico que irradia con una polarización lineal vertical. En el extremo receptor, el campo es captado por una antena tipo bocina piramidal (\textit{Horn}). Para cuantificar la energía recibida, la antena se acopla a un diodo rápido Schottky montado en posición vertical, el cual rectifica la señal de RF para ser leída por un amperímetro analógico.

	\begin{figure}[H]
		\centering
		\begin{subfigure}{0.35\textwidth}
			\centering
			\includegraphics[height=5.5cm]{images/image27.png}
			\caption{Transmisor parabólico}
		\end{subfigure}
		\hfill
		\begin{subfigure}{0.35\textwidth}
			\centering
			\includegraphics[height=5.5cm]{images/image37.png}
			\caption{Receptor de bocina}
		\end{subfigure}
		\caption{Antenas de transmisión y recepción en el banco.}
		\label{fig:antenas_banco}
	\end{figure}"""

content = re.sub(old_banco, new_banco, content)

# 6. Añadir sección LNB al final antes de Resultados
lnb_section = r"""
	\subsection{Mediciones adicionales en banco con LNB}

    Como actividad extra, se integró en la recepción un LNB (\textit{Low Noise Block}) dual para bajar en frecuencia la señal de la banda Ku y permitir su análisis con un analizador de espectro de menor rango.
    Este dispositivo capta las frecuencias y las amplifica con bajo ruido, realizando una conversión a frecuencia intermedia (IF) mediante un oscilador local (OL).

	\begin{figure}[H]
		\centering
		\begin{subfigure}{0.65\textwidth}
			\centering
			\includegraphics[width=0.9\linewidth]{images/image29.png}
			\caption{Conexión del LNB al montaje}
		\end{subfigure}
		\hfill
		\begin{subfigure}{0.3\textwidth}
			\centering
			\includegraphics[width=0.9\linewidth]{images/image22.jpg}
			\caption{LNB dual}
		\end{subfigure}
		\caption{Integración de LNB en el banco experimental.}
		\label{fig:lnb}
	\end{figure}

    \subsubsection{Inyector y filtro activo LC (Winegard PS-1403)}
    Para la correcta operación del LNB, se empleó un filtro inyector de continua (Winegard PS-1403) actuando como \textit{bias tee}. Esto permitió alimentar el LNB con continua sin afectar el paso de la señal de RF hacia el analizador de espectro, protegiendo sus puertos de entrada.

	\begin{figure}[H]
		\centering
		\includegraphics[width=0.7\textwidth]{images/image26.jpg}
		\caption{Filtro activo LC empleado para aislamiento y alimentación.}
		\label{fig:filtro_winegard}
	\end{figure}

	\begin{figure}[H]
		\centering
		\includegraphics[width=0.6\textwidth]{images/image28.png}
		\caption{Esquema de conexiones con el analizador de espectro.}
		\label{fig:esquema_general}
	\end{figure}

    \subsubsection{Lecturas de frecuencia convertida}
    En el analizador de espectro se identificó correctamente el tono convertido a frecuencia intermedia $f_{IF} \approx 1{,}9$\,GHz.

	\begin{figure}[H]
		\centering
		\includegraphics[width=0.6\textwidth]{images/image33.jpg}
		\caption{Tono medido en el analizador de espectro.}
		\label{fig:medicion_espectro}
	\end{figure}

    Sabiendo la frecuencia transmitida por el oscilador ($f_{RF}$) y la frecuencia detectada en el analizador ($f_{IF}$), se verifica empíricamente la frecuencia del oscilador local ($f_{OL}$) interno del LNB utilizando la relación fundamental de los mezcladores:
    \begin{equation}
        f_{OL} = f_{RF} - f_{IF} = 11{,}65\,\text{GHz} - 1{,}90\,\text{GHz} = 9{,}75\,\text{GHz}
    \end{equation}

	% ============================================================
	%  7. RESULTADOS Y CONCLUSIONES
	% ============================================================
"""

content = content.replace(
    r"""	% ============================================================
	%  7. RESULTADOS Y CONCLUSIONES
	% ============================================================
""", lnb_section)

# 7. Quitar el comando que define TODO si quedó por ahí, y lo corregimos globalmente por si no lo encontramos
content = content.replace(
    r"""\newcommand{\TODO}[1]{\textcolor{placeholder}{\textbf{[TODO: #1]}}}""", 
    r""
)

with open(r'c:\Users\pabli\OneDrive\Desktop\pll\tex\main.tex', 'w', encoding='utf-8') as f:
    f.write(content)

print("Edits applied successfully.")
