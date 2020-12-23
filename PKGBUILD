# Maintainer: <jacob.tye@outlook.com>
pkgname=angel
pkgver=v1.0.0_beta
pkgrel=1
arch=('x86_64' 'arm' 'armv6h' 'armv7h' 'aarch64')
pkgdesc="A Reddit client for Linux"
url="https://github.com/Starkiller645/angelv2"
license=('GPL')
depends=('qt5-base' 'nlohmann-json' 'qt5-multimedia' 'qt5-webengine' 'qt5-websockets')
source=("$pkgname-$pkgver.tar.gz::https://github.com/Starkiller645/angelv2/releases/download/$pkgver/$pkgname-$pkgver.tar.gz")
sha256sums=('3357a3c2e8ed886ca19fc44eb4bf3ef890f9709d896d2a7574405a57736ee931')
build() {
	mkdir build
	cd build
	cmake ..
	make
}

package() {
	cd build
	make DESTDIR="$pkgdir" install
}
