#include <QApplication>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QStyle>
#include <QVBoxLayout>
#include <QWidget>
#include <array>

// Cola circular FIFO: capacidad fija de 6 trabajos, enviados desde PC1...PC5.
class ColaImpresion {
public:
    static constexpr int CAPACIDAD = 6;

    bool estaVacia() const { return cantidad_ == 0; }
    bool estaLlena() const { return cantidad_ == CAPACIDAD; }
    int cantidad() const { return cantidad_; }

    bool agregarTrabajo(int computadora) {
        if (estaLlena() || computadora < 1 || computadora > 5) return false;
        const int final = (frente_ + cantidad_) % CAPACIDAD;
        trabajos_[final] = computadora;
        ++cantidad_;
        return true;
    }

    // Devuelve 0 si no existe un trabajo pendiente.
    int imprimirDocumento() {
        if (estaVacia()) return 0;
        const int computadora = trabajos_[frente_];
        frente_ = (frente_ + 1) % CAPACIDAD;
        --cantidad_;
        return computadora;
    }

    int pendiente(int indice) const {
        if (indice < 0 || indice >= cantidad_) return 0;
        return trabajos_[(frente_ + indice) % CAPACIDAD];
    }

private:
    std::array<int, CAPACIDAD> trabajos_{};
    int frente_ = 0;
    int cantidad_ = 0;
};

class VentanaPrincipal : public QMainWindow {
public:
    VentanaPrincipal() {
        setWindowTitle("Cola de Impresion | Simulador FIFO");
        setMinimumSize(820, 580);
        resize(960, 650);

        auto *central = new QWidget(this);
        central->setObjectName("central");
        auto *layout = new QVBoxLayout(central);
        layout->setContentsMargins(30, 28, 30, 28);
        layout->setSpacing(20);

        auto *eyebrow = new QLabel("ESTRUCTURAS DE DATOS  /  C++");
        eyebrow->setObjectName("eyebrow");
        layout->addWidget(eyebrow);

        auto *titulo = new QLabel("Cola de impresion");
        titulo->setObjectName("title");
        layout->addWidget(titulo);

        auto *subtitulo = new QLabel("5 computadoras  ·  1 impresora compartida  ·  Capacidad: 6 trabajos  ·  FIFO");
        subtitulo->setObjectName("subtitle");
        layout->addWidget(subtitulo);

        auto *stats = new QHBoxLayout;
        auto *contadorLabel = new QLabel("TRABAJOS PENDIENTES");
        contadorLabel->setObjectName("statLabel");
        contador_ = new QLabel("0 / 6");
        contador_->setObjectName("statNumber");
        stats->addWidget(contadorLabel);
        stats->addStretch();
        stats->addWidget(contador_);
        layout->addLayout(stats);

        auto *queueLabel = new QLabel("ORDEN DE ATENCION     PRIMERO  →  ULTIMO");
        queueLabel->setObjectName("queueLabel");
        layout->addWidget(queueLabel);

        auto *slots = new QGridLayout;
        slots->setHorizontalSpacing(10);
        slots->setVerticalSpacing(10);
        for (int i = 0; i < ColaImpresion::CAPACIDAD; ++i) {
            auto *slot = new QLabel;
            slot->setObjectName("queueSlot");
            slot->setAlignment(Qt::AlignCenter);
            slot->setMinimumSize(115, 118);
            slot->setWordWrap(true);
            slots_.at(i) = slot;
            slots->addWidget(slot, 0, i);
        }
        layout->addLayout(slots);

        auto *controls = new QHBoxLayout;
        controls->setSpacing(12);
        auto *add = new QPushButton("+ Agregar trabajo");
        add->setObjectName("addButton");
        auto *print = new QPushButton("Imprimir primero");
        print->setObjectName("printButton");
        auto *show = new QPushButton("Mostrar cola");
        show->setObjectName("secondaryButton");
        auto *clear = new QPushButton("Reiniciar");
        clear->setObjectName("secondaryButton");
        controls->addWidget(add);
        controls->addWidget(print);
        controls->addWidget(show);
        controls->addWidget(clear);
        layout->addLayout(controls);

        estado_ = new QLabel("Lista para recibir trabajos. La siguiente solicitud entrara al final de la cola.");
        estado_->setObjectName("status");
        estado_->setWordWrap(true);
        layout->addWidget(estado_);

        auto *historyTitle = new QLabel("REGISTRO DE OPERACIONES");
        historyTitle->setObjectName("queueLabel");
        layout->addWidget(historyTitle);
        historial_ = new QLabel("Sin operaciones todavia.");
        historial_->setObjectName("log");
        historial_->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        historial_->setWordWrap(true);
        auto *scroll = new QScrollArea;
        scroll->setObjectName("scrollLog");
        scroll->setWidgetResizable(true);
        scroll->setMinimumHeight(110);
        scroll->setWidget(historial_);
        layout->addWidget(scroll);

        setCentralWidget(central);
        setStyleSheet(R"(
            QMainWindow, QWidget#central { background: #0B1220; color: #F3F6FC; }
            QLabel#eyebrow, QLabel#queueLabel, QLabel#statLabel { color: #94A3B8; font-size: 12px; font-weight: 700; letter-spacing: 1px; }
            QLabel#title { color: #F8FAFC; font-size: 32px; font-weight: 800; }
            QLabel#subtitle { color: #A9B8CA; font-size: 13px; }
            QLabel#statNumber { color: #38BDF8; font-size: 26px; font-weight: 800; }
            QLabel#queueSlot { background: #18253A; border: 1px solid #334155; border-radius: 12px; color: #64748B; font-size: 15px; font-weight: bold; }
            QLabel#queueSlot[filled="true"] { background: #12354A; border: 2px solid #22D3EE; color: #E0F2FE; }
            QLabel#queueSlot[first="true"] { background: #174637; border: 2px solid #4ADE80; color: #DCFCE7; }
            QPushButton { min-height: 45px; border-radius: 9px; padding: 0 12px; font-weight: 700; font-size: 13px; }
            QPushButton#addButton { color: #082F49; background: #67E8F9; }
            QPushButton#addButton:hover { background: #A5F3FC; }
            QPushButton#printButton { color: #052E16; background: #86EFAC; }
            QPushButton#printButton:hover { background: #BBF7D0; }
            QPushButton#secondaryButton { color: #CBD5E1; background: #26354C; }
            QPushButton#secondaryButton:hover { background: #334155; }
            QLabel#status { color: #CBD5E1; font-size: 13px; padding: 8px 0; }
            QScrollArea#scrollLog { background: #121E30; border: 1px solid #26354C; border-radius: 10px; }
            QLabel#log { color: #CDD8E7; background: #121E30; padding: 14px; font-size: 13px; }
        )");

        connect(add, &QPushButton::clicked, this, [this] {
            if (cola_.estaLlena()) {
                aviso("La cola de impresion esta LLENA.");
                return;
            }
            const int pc = QRandomGenerator::global()->bounded(1, 6);
            cola_.agregarTrabajo(pc);
            registrar(QString("Trabajo agregado desde PC%1.").arg(pc));
            actualizar();
        });
        connect(print, &QPushButton::clicked, this, [this] {
            if (cola_.estaVacia()) {
                aviso("La cola de impresion esta VACIA.");
                return;
            }
            const int pc = cola_.imprimirDocumento();
            registrar(QString("Imprimiendo documento de PC%1. Trabajo retirado del frente.").arg(pc));
            actualizar();
        });
        connect(show, &QPushButton::clicked, this, [this] {
            if (cola_.estaVacia()) { aviso("La cola de impresion esta VACIA."); return; }
            QString detalle;
            for (int i = 0; i < cola_.cantidad(); ++i)
                detalle += QString("%1. PC%2\n").arg(i + 1).arg(cola_.pendiente(i));
            QMessageBox::information(this, "Trabajos pendientes (FIFO)", detalle);
            registrar("Consulta de trabajos pendientes.");
        });
        connect(clear, &QPushButton::clicked, this, [this] {
            cola_ = ColaImpresion{};
            registrar("Cola reiniciada.");
            actualizar();
        });
        actualizar();
    }

private:
    void registrar(const QString &mensaje) {
        estado_->setText(mensaje);
        if (historial_->text() == "Sin operaciones todavia.") historial_->clear();
        historial_->setText("• " + mensaje + "\n" + historial_->text());
    }
    void aviso(const QString &mensaje) {
        registrar(mensaje);
        QMessageBox::warning(this, "Aviso de la cola", mensaje);
    }
    void actualizar() {
        contador_->setText(QString("%1 / 6").arg(cola_.cantidad()));
        for (int i = 0; i < ColaImpresion::CAPACIDAD; ++i) {
            const bool lleno = i < cola_.cantidad();
            QLabel *slot = slots_[i];
            slot->setText(lleno
                ? QString("TURNO %1\n\nPC%2").arg(i + 1).arg(cola_.pendiente(i))
                : QString("ESPACIO %1\n\nLibre").arg(i + 1));
            slot->setProperty("filled", lleno);
            slot->setProperty("first", lleno && i == 0);
            slot->style()->unpolish(slot);
            slot->style()->polish(slot);
            slot->update();
        }
    }
    ColaImpresion cola_;
    std::array<QLabel *, ColaImpresion::CAPACIDAD> slots_{};
    QLabel *contador_ = nullptr;
    QLabel *estado_ = nullptr;
    QLabel *historial_ = nullptr;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    VentanaPrincipal ventana;
    ventana.show();
    return app.exec();
}
