# Maintainer: <jacob.tye@outlook.com>
pkgname=angel
pkgver=v1.0.0_beta
pkgrel=1
arch=('x86_64' 'arm' 'armv6h' 'armv7h' 'aarch64')
pkgdesc="A Reddit client for Linux"
url="https://github.com/Starkiller645/angelv2"
license=('GPL')
depends=('qt5' 'nlohmann-json')
source=("$pkgname-$pkgver.tar.gz::https://github.com/Starkiller645/angelv2/releases/download/$pkgver/$pkgname-$pkgver.tar.gz")
build() {
	cd "$srcdir/$pkgname-$pkgver"
	mkdir build
	cd build
	cmake ..
	make
}

package() {
	cd "$srcdir/$pkgname-$pkgver"
	make DESTDIR="$pkgdir" install
}
