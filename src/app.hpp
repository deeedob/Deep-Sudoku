#pragma once
#include <QtQml/qqmlregistration.h>
#include <QPointer>
#include <memory>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickImageProvider>
#include "board_model.hpp"
#include "deep_solver.hpp"

/*!
 * @defgroup QT QT
 */
/*!
 * @defgroup AI neural-network
 */
/*!
 * @defgroup PROC image-Processing
 */
/*!
 * @defgroup QML Qml-Files
 * @ingroup QT
 * */

/*!
 * @brief this namespace contains the whole c++ side.
 */
namespace dsdk
{
	/*!
	 * @ingroup QT
	 * @brief Managing the runtime of Deep-Sudoku
	 * @note This Class is implemented as singleton and added as [qml_add_qml_module()](https://doc-snapshots.qt.io/qt6-dev/qt-add-qml-module.html)
	 * @author Dennis Oberst
	 */
	class App : public QObject
	{
	Q_OBJECT
		QML_ELEMENT
		QML_SINGLETON
		Q_PROPERTY( BoardModel* model READ getModel CONSTANT )
	public:
		static App* create( QQmlEngine* engine, QJSEngine* jsEngine );
		App() = delete;
		App( const App& ) = delete;
		App& operator=( const App& ) = delete;
		App( App&& ) = delete;
		App& operator=( App&& ) = delete;
		/*!
		 * @brief read only access for the qml-side
		 * @return the board model
		 */
		BoardModel* getModel() const;
		/*!
		 * @brief asynchronous method to grab an image from QQuickItem
		 * @param src QQuickItem sent from QML
		 * @return true if the processing was successfull
		 * @note This function grabs the image from the current scene and uses the
		 * current width and height
		 * @deprecated please use solveGame()
		 */
		Q_INVOKABLE bool getQmlImage( QQuickItem* src );
		/*!
		 * @brief Generates an image from the url and processes it with the DeepSolver Class
		 * @note this enables the use of the full resolution of the image
		 * @param url url containing the image. Either from QML or filesystem
		 * @return trie if the processing was successfull
		 */
		Q_INVOKABLE bool solveGame( const QUrl& url );
	private:
		explicit App( QObject* parent = nullptr );
		~App() override = default;
	
	private:
		QPointer<BoardModel> m_boardModel;
		std::unique_ptr<DeepSolver> m_deepSolver;
	};
}